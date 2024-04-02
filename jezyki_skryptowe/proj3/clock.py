import sys
import argparse

from clock_qt import launch_qt


def main():
    parser = argparse.ArgumentParser(description='Uruchom aplikację zegara.')
    parser.add_argument('--qt', '-q', action='store_true', help='Uruchom aplikację z interfejsem w PyQT5.')
    parser.add_argument('--gtk', '-g', action='store_true', help='Uruchom aplikcją z interfejsem w PyGTK3.')
    args = parser.parse_args()

    if args.qt:
        launch_qt()
    elif args.gtk:
        print("Do nothing with GTK for now.")
        sys.exit(0)

    launch_qt()


if __name__ == '__main__':
    main()
