from argparse import ArgumentParser
from LSB.LeastSignificantBitHiding import hide
from LSB.LeastSignificantBitUnhiding import reveal


def handle_hiding(algorithm, message, source_path, result_name):
    if algorithm == "LSB":
        hide(source_path, message, result_name)


def handle_unhiding(algorithm, source):
    if algorithm == "LSB":
        print(reveal(source))


if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument(
        "-H",
        "--hide",
        help="If you want hide something use this flag.",
        action="store_true",
    )

    parser.add_argument(
        "-u",
        "--unhide",
        help="Reveal a Message Which is Covered in source",
        action="store_true",
    )

    parser.add_argument(
        "-a",
        "--algorithm",
        help="Algorithm Which Will be Used for Hiding or Unhiding Data. (default: %(default)s)",
        choices=["LSB"],
        default="LSB",
    )

    parser.add_argument(
        "-m", "--message", help="Message That You Want Hide It.", action="store"
    )

    parser.add_argument(
        "-s",
        "--source",
        help="Path of Source That Message Should Hide in It. For Now It Should be an Image",
        action="store",
    )

    parser.add_argument(
        "-rn",
        "--result_name",
        help="Name of Result Media. We Will Choose It Format.",
        action="store",
    )

    arguments = parser.parse_args()
    if arguments.hide:
        handle_hiding(
            arguments.algorithm,
            arguments.message,
            arguments.source,
            arguments.result_name,
        )
    if arguments.unhide:
        handle_unhiding(arguments.algorithm, arguments.source)
