import argparse
import time
import search_string

def main():
    parser = argparse.ArgumentParser(description='Search for a string in a file.')
    parser.add_argument('filename', help='the name of the file to search in')
    parser.add_argument('pattern', help='the string to search for')
    args = parser.parse_args()

    time_begin = time.time()
    try:
        result = search_string.search_string(args.filename, args.pattern)
        print(f"Found string at {result}")
    except:
        print("Failed to find string in file")
    time_end = time.time()
    print(f"Time taken: {time_end - time_begin} seconds")
    
if __name__ == "__main__":
    main()