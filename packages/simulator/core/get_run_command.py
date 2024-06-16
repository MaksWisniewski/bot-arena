import os
import platform


def get_run_command(file: str, args: str) -> list[str]:
    file_name, ext = os.path.splitext(os.path.basename(file))
    path = os.path.dirname(file)
    ext = ext.lower()

    if ext == ".py":  # Python
        python_command = "python" if platform.system().lower() == "windows" else "python3"
        return [python_command, file] + args.split()

    elif ext == ".exe" or ext == "":  # Binary code
        return [file] + args.split()
    else:
        raise ValueError(f"Unsupported file type for running: {ext}")
