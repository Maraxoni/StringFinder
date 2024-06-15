import subprocess
import sys
import os

def main():
    if len(sys.argv) != 3:
        print("Usage: python StringFinder_Menu.py <filename> <search_term>")
        sys.exit(1)

    filename = sys.argv[1]
    search_term = sys.argv[2]

    exe_path = os.path.join(os.path.dirname(__file__), 'StringFinder_Search.exe')

    if not os.path.exists(exe_path):
        print(f"Executable not found: {exe_path}")
        sys.exit(1)

    try:
        result = subprocess.run([exe_path, filename, search_term], check=True, capture_output=True, text=True)
        print(result.stdout)
    except subprocess.CalledProcessError as e:
        print("An error occurred:", e.stderr)
    except FileNotFoundError as e:
        print(f"File not found: {e.filename}")

if __name__ == "__main__":
    main()