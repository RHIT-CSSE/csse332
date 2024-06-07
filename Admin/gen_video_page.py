#!/usr/bin/env python

import csv
import logging
import argparse
from datetime import datetime, timedelta


def parse_duration(duration):
    """
    Parse a duration string only focusing on minutes and seconds.

    :param: duration: The duration string to parse.
    :type: duration: Str

    :return: A datetime timedelta object from the parsed string
    """
    dt = datetime.strptime(duration, "%M:%S:%f")
    return timedelta(minutes=dt.minute,
                     seconds=dt.second, microseconds=dt.microsecond)


def process_entries(reader):
    """
    Process the dictionnaries obtained from the CSV file.

    :param: reader: The dictionnary reader obtained from the file.
    """
    for r in reader:
        # obtain the entry
        try:
            # name = r['Video']
            duration = r['Duration']
            link = r['Sharing link']
            desc = r['Description']

            print(f'''
<h4>{desc} (<code>{parse_duration(duration)}</code>)</h4>
<p>{link}</p>
<p></p>
            ''')
        except KeyError:
            logging.error('''Malformed csv file, please regenerate and try
                          again...''')
            return


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO)

    parser = argparse.ArgumentParser(
        prog="gen_video_page.py",
        description="Generate Moodle video page based on given csv file.")

    parser.add_argument("-f", "--file",
                        type=str,
                        required=True,
                        help='''
                            The path to the csv file to parse for generating
                            the text.
                            ''')

    args = parser.parse_args()
    with open(args.file, 'r') as f:
        reader = csv.DictReader(f)

        process_entries(reader)
