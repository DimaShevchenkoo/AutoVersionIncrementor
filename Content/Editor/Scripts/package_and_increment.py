import threading, os, subprocess, configparser, unreal, queue

log_queue = queue.Queue()
status_queue = queue.Queue()

IGNORED_PREFIXES = [
    "LogChaosDD",
    "LogWorldPartition",
    "LogShaderCompilers",
    "LogPython"
]

def should_log_line(line: str) -> bool:
    return not any(line.startswith(prefix) for prefix in IGNORED_PREFIXES)

def process_queues(delta_time):
    while not log_queue.empty():
        unreal.log("[UAT] " + log_queue.get())
    while not status_queue.empty():
        status, archive_path, platform = status_queue.get()
        if status == "success":
            new_ver = increment_version()
            rename_build_folder(archive_path, platform)
            unreal.log(f"[UAT] Packaging finished successfully. Version updated to {new_ver}")
        elif status == "fail":
            unreal.log_error("[UAT] Packaging failed! Version not incremented.")

def increment_version():
    ini_path = os.path.join(unreal.Paths.project_config_dir(), "DefaultGame.ini")
    config = configparser.ConfigParser()
    config.optionxform = str
    config.read(ini_path)

    section = "/Script/EngineSettings.GeneralProjectSettings"
    if section not in config:
        config[section] = {}

    if "ProjectVersion" not in config[section]:
        config[section]["ProjectVersion"] = "1.0.0.0"

    version = config[section]['ProjectVersion']
    parts = list(map(int, version.split('.')))

    while len(parts) < 4:
        parts.append(0)

    index = len(parts) - 1
    parts[index] += 1
    while index > 0 and parts[index] >= 10:
        parts[index] = 0
        index -= 1
        parts[index] += 1

    while len(parts) < 4:
        parts.append(0)

    new_version = '.'.join(map(str, parts))
    config[section]['ProjectVersion'] = new_version

    with open(ini_path, 'w') as configfile:
        config.write(configfile)

    unreal.SystemLibrary.execute_console_command(None, "config reload")
    return new_version

def run_uat_async(args, archive_path, platform):
    def worker():
        process = subprocess.Popen(
            args,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            shell=False,
            bufsize=1
        )
        for line in iter(process.stdout.readline, ''):
            clean_line = line.strip()
            if clean_line and should_log_line(clean_line):
                unreal.log("[UAT] " + clean_line)
        process.stdout.close()
        process.wait()
        status_queue.put(("success" if process.returncode == 0 else "fail", archive_path, platform))
    threading.Thread(target=worker, daemon=True).start()

def package_and_increment(platform, config, archive_path):
    while not status_queue.empty():
        status_queue.get()

    project_path = unreal.Paths.convert_relative_path_to_full(unreal.Paths.get_project_file_path())
    engine_dir = os.path.abspath(unreal.Paths.engine_dir())
    uat_path = os.path.normpath(os.path.join(engine_dir, "Build", "BatchFiles", "RunUAT.bat"))
    args = [
        uat_path,
        "BuildCookRun",
        f"-project={project_path}",
        "-noP4",
        f"-clientconfig={config}",
        "-nocompileeditor",
        "-archive",
        f"-archivedirectory={os.path.normpath(archive_path)}",
        "-package",
        "-build",
        "-cook",
        "-pak",
        "-iterate",
        "-prereqs",
        "-skipstage",
        f"-targetplatform={platform}",
        "-verbose"
    ]
    run_uat_async(args, archive_path, platform)

unreal.register_slate_post_tick_callback(process_queues)

def get_project_version():
    ini_path = os.path.join(unreal.Paths.project_config_dir(), "DefaultGame.ini")
    config = configparser.ConfigParser()
    config.read(ini_path)
    section = "/Script/EngineSettings.GeneralProjectSettings"
    return config[section]['ProjectVersion']

def rename_build_folder(archive_path, platform):
    version = get_project_version()
    build_folder_name = get_platform_build_folder_name(platform)
    build_dir = os.path.join(os.path.normpath(archive_path), build_folder_name)
    if not os.path.exists(build_dir):
        unreal.log_error(f"[UAT] Build folder {build_dir} not found!")
        return get_platform_build_folder_name(platform)

    base_dir = os.path.dirname(build_dir)
    new_name = f"{build_folder_name}_{version}"
    new_path = os.path.join(base_dir, new_name)

    counter = 1
    while os.path.exists(new_path):
        new_path = os.path.join(base_dir, f"{build_folder_name}_{version}_{counter}")
        counter += 1

    os.rename(build_dir, new_path)
    unreal.log(f"[UAT] Build folder renamed to {new_path}")
    return new_path

def get_platform_build_folder_name(platform):
    mapping = {
        "Win64": "Windows",
        "Linux": "Linux",
        "PS4": "PS4",
        "PS5": "PS5",
        "XboxOne": "XboxOne",
        "XSX": "XboxSeriesX",
        "Android": "Android",
        "IOS": "IOS",
        "Mac": "Mac",
        "Nintendo": "Nintendo"
    }
    return mapping.get(platform, platform)