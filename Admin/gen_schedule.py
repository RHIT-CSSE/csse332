# %%
import yaml
import pprint as pp
import csv
import logging
import os
from datetime import datetime as dt


# %%
def find_sources_root(tree_root='csse332-202130'):
    """
    Find the root directory containing the sources for the website.

    :return: Returns the real path of the sources root.
    :rtype: str
    """
    curr_dir = os.path.realpath(os.curdir).split('/')
    curr_dir[0] = '/'  # adjust to have the actual path at the end
    while curr_dir[-1] != tree_root and len(curr_dir) > 0:
        curr_dir.pop()

    logging.info("Root of source tree is {}".format(os.path.join(*curr_dir)))
    return os.path.join(*curr_dir)


def parse_time_str(time_string):
    """
    Parse a time string and return a datetime object.

    :param: time_string: The string to parse.
    :type: time_string: Str

    :return: The parsed time object.
    :rtype: datetime.datetime
    """
    ret_obj = None

    schedule_fmt = '%Y-%m-%d %I:%M'
    assignment_fmt = '%Y-%m-%d %H:%M:%S'
    try:
        ret_obj = dt.strptime(time_string, schedule_fmt)
    except ValueError as e:
        logging.debug("Failed to parse {} as {}, trying {}".format(
            time_string, schedule_fmt, assignment_fmt
        ))
        try:
            ret_obj = dt.strptime(time_string, assignment_fmt)
        except ValueError as e:
            logging.error(
                "Failed to parse datetime string {}".format(time_string))

    return ret_obj


def determine_starting_week(schedule):
    """
    Determine the starting week of the term.

    :param: schedule:   The schedule breakdown as a list of dictionaries.
    :type: schedule: [{str:str}]

    :return: the number of the week to start from
    :rtype: int
    """
    starting_session = schedule[0]['date']
    start_time = parse_time_str(starting_session)
    if not start_time:
        raise RuntimeError("Could not find starting week number")
    if start_time.day >= 2:
        return 0
    else:
        return 1

def count_weeks(schedule, starting_week):
    """
    Count the number of weeks in the quarter and determine their spans.

    :param: schedule: The list of assignments sorted by date.
    :param: starting_week: The number of the starting week.

    :type: schedule: [{Str:Str}]
    :type: starting_week: int

    :return: A list of integers that represents the number of sessions/assignments
        in the at particular week
    :rtype: [int]
    """
    # at most, there are 11 weeks!
    week_counter = [0]*11

    current_week = starting_week
    prev_week = parse_time_str(schedule[0]['date']).isocalendar()[1]
    for session in schedule:
        session_date = parse_time_str(session['date'])
        week_number = session_date.isocalendar()[1]
        if (week_number != prev_week):
            current_week += 1

        if (current_week >= len(week_counter)):
            logging.error("What ? {}".format(current_week))
        week_counter[current_week] += 1
        session['week'] = current_week
        prev_week = week_number

    return week_counter

def write_schedule_header(f):
    """
    Write the header for the schedule in the markdown file.

    :param: f: the file to write to.
    :type: f: file

    :return: None.
    """
    header = """---
layout = page
title = Class Schedule
---

# Class Schedule
<table>
<colgroup>
<col width="10%" />
<col width="10%" />
<col width="20%" />
<col width="40%" />
<col width="20%" />
</colgroup>"""
    f.write(header)

def write_table_header(f):
    """
    Write the header for the table in the markdown file.

    :param: f: the file to write to.
    :type: f: file

    :return: None.
    """
    f.write("<thead>\n")
    f.write("<tr class=\"header\">\n")
    f.write("<th> Week </th>\n")
    f.write("<th> Session </th>\n")
    f.write("<th> Reading </th>\n")
    f.write("<th> Topics </th>\n")
    f.write("<th< Material </th>\n")
    f.write("</tr>\n")
    f.write("</thead>\n")



# %%
if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)

    source_root = find_sources_root()
    date_convert_path = os.path.join(source_root, '_data', 'date_convert.yml')
    assignments_path = os.path.join(source_root, '_data', 'assignments.csv')
    schedule_path = os.path.join(source_root, '_data', 'schedule.yml')

    # schedule is a list of dictionnaries.
    schedule = []
    with open(date_convert_path, 'r') as f:
        schedule = yaml.full_load(f)

    with open(assignments_path, 'r') as f:
        reader = csv.DictReader(f)

        list_of_dicts = [x for x in reader if x['name'][0] != '#']
        schedule.extend(list_of_dicts)

    logging.info("There are {} calendar entries between assignments and class sessions"
                 .format(len(schedule)))

    schedule = sorted(schedule, key=lambda k: k['date'])

    starting_week = determine_starting_week(schedule)
    logging.info("Starting week is {}".format(starting_week))
    num_of_sessions_per_week = count_weeks(schedule, starting_week)


# %%
