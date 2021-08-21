# %%
import yaml
import pprint as pp
import csv
import logging
import os
from datetime import datetime as dt


# %%
def find_sources_root(tree_root='csse332'):
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
    except ValueError:
        logging.debug("Failed to parse {} as {}, trying {}".format(
            time_string, schedule_fmt, assignment_fmt
        ))
        try:
            ret_obj = dt.strptime(time_string, assignment_fmt)
        except ValueError:
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
    week_counter = [0]*12

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
layout: page
title: Class Schedule
---

**Jump to week**: **[0](#w0) [1](#w1) [2](#w2) [3](#w3) [4](#w4) [5](#w5) [6](#w6) [7](#w7) [8](#w8) [9](#w9) [10](#w10)**

<table>
<colgroup>
<col width="10%" />
<col width="15%" />
<col width="25%" />
<col width="25%" />
<col width="25%" />
</colgroup>"""
    f.write(header)


def write_table_header(f):
    """
    Write the header for the table in the markdown file.

    :param: f: the file to write to.
    :type: f: file

    :return: None.
    """
    # TODO: Make this one call to write. This is bad but helpful when debugging
    f.write("<thead>\n")
    f.write("<tr class=\"header\">\n")
    f.write("<th style=\"background-color:#167F92;color:#FFF;\"> Week </th>\n")
    f.write("<th style=\"background-color:#167F92;color:#FFF;\"> Session </th>\n")
    f.write("<th style=\"background-color:#167F92;color:#FFF;\"> Reading </th>\n")
    f.write("<th style=\"background-color:#167F92;color:#FFF;\"> Topics </th>\n")
    f.write("<th style=\"background-color:#167F92;color:#FFF;\"> Material </th>\n")
    f.write("</tr>\n")
    f.write("</thead>\n")

def get_session_date(session):
    """
    Get the session date in datetime format

    :param: session: The dictionary representing the session
    :type: session: {Str:Str}

    :return: A datetime.datetime representation of the session's date.
    :rtype: datetime.datetime
    """
    try:
        session_date = parse_time_str(session['date'])
        return session_date
    except KeyError:
        logging.error("Could not parse a session's date, process failing...")
        raise RuntimeError("Failed to parse session date!")

def is_class_session(session):
    """
    Check is a session has a class_session type.

    :param: session: The session to check.
    :type: session: {Str:Str}

    :return: True if the session is of class_session type, False otherwise.
    :rtype: bool
    """
    try:
        session_type = session['type']
        return session_type == 'class_session'
    except KeyError:
        return False

def start_column(f, options=None):
    if options:
        f.write("<td markdown=\"span\" {}>\n".format(options))
    else:
        f.write("<td markdown=\"span\">\n")


def end_column(f):
    f.write("</td>\n")



def write_table_body(f, schedule, num_of_sessions_per_week, classes):
    """
    Write the table body based on the type of the session and so on.

    :param: f: the file we are writing to.
    :param: schedule: the list of entries as a list of dictionaries.
    :param: num_of_sessions_per_week: the list detailing the number of sessions each week contains.
    :param: classes: the list of class sessions with materials and links.

    :type: f: File
    :type: schedule: [{Str:Str}]
    :type: num_of_sessions_per_week: [int]

    :return: None
    """
    previous_week = -1
    for session in schedule:
        date = get_session_date(session)
        week = session['week']
        f.write("<tr>\n")

        # check if we need a new week column
        if week != previous_week:
            f.write("""<td rowspan="{1}" markdown="span" style=\"font-size:xx-large;\"><a name=\"w{0}\">{0}</a></td>\n""".format(
                week, num_of_sessions_per_week[week]))


        # now dump the actual stuff
        previous_week = week
        if is_class_session(session):
            session_num = session['class_num']
            # write the date
            start_column(f)
            f.write("{} <br/> {}".format(session_num, date.strftime("%a, %b %d %Y")))
            end_column(f)

            try:
                logging.debug("Checking session {}".format(session_num))
                class_content = classes[session_num]
                # check for reading material
                if class_content['reading']:
                    start_column(f, 'style=\"text-align:left\"')
                    f.write("{::nomarkdown}<ul style=\"margin:0;padding:.5rem\">\n")
                    for item in class_content['reading']:
                        f.write("<li> {} </li>\n".format(item))
                    f.write("</ul>{:/}\n")
                    end_column(f)
                else:
                    start_column(f)
                    end_column(f)

                # check for topics
                if class_content['topics']:
                    start_column(f, 'style=\"text-align:left\"')
                    # f.write("{::nomarkdown}<ul>\n")
                    f.write("{::nomarkdown}<ul style=\"margin:0;padding:.5rem\">\n")
                    for topic in class_content['topics']:
                        f.write("<li> {} </li>\n".format(topic))
                    f.write("</ul>{:/}\n")
                    end_column(f)
                else:
                    start_column(f)
                    end_column(f)

                # check for materials
                start_column(f)

                if class_content['materials']:
                    if type(class_content['materials']) is str:
                        f.write(class_content['materials'] + "<br/>")
                    else:
                        # f.write("{::nomarkdown}<ul>\n")
                        f.write("{::nomarkdown}<ul style=\"margin:0;padding:.5rem\">\n")
                        for material in class_content['materials']:
                            f.write("<li>{}</li>".format(material))
                        f.write("</ul>{:/}\n")

                # finally check for others
                if class_content['other']:
                    f.write('<br/>'.join(class_content['other']))

                end_column(f)

            except KeyError:
                f.write("<td markdown=\"span\" colspan=\"3\">N/A</td>\n")
        else:
            # assignment
            try:
                assignment_name = session['name']
                assignment_dir = session['dirname']
                assignment_date = get_session_date(session)
                assignment_box = session['moodle']
                assignment_grader = session['grader']

                if assignment_dir is not None and assignment_dir != "":
                    if assignment_grader is not None and assignment_grader != '':
                        assignment_grader = "<font color=\"#aaa\"> grader: @{} </font>".format(assignment_grader)
                        f.write("<td markdown=\"span\" colspan=\"4\"> [{}]({{{{ site.baseurl }}}}/docs/{}) DUE {} {} {} </td>".format(
                            assignment_name, assignment_dir,
                            assignment_date.strftime("%a, %b %d %Y %H:%M"),
                            assignment_box, assignment_grader
                        ))
                    else:
                        f.write("<td markdown=\"span\" colspan=\"4\"> [{}]({{{{ site.baseurl }}}}/docs/{}) DUE {} {}</td>".format(
                            assignment_name, assignment_dir,
                            assignment_date.strftime("%a, %b %d %Y %H:%M"), assignment_box
                        ))
                else:
                    if assignment_grader is not None and assignment_grader != '':
                        assignment_grader = "<font color=\"#aaa\"> grader: @{} </font>".format(assignment_grader)
                        f.write("<td markdown=\"span\" colspan=\"4\"> {} DUE {} {} {} </td>".format(
                            assignment_name,
                            assignment_date.strftime("%a, %b %d %Y %H:%M"),
                            assignment_box, assignment_grader
                        ))
                    else:
                        f.write("<td markdown=\"span\" colspan=\"4\"> {} DUE {} {}</td>".format(
                            assignment_name,
                            assignment_date.strftime("%a, %b %d %Y %H:%M"), assignment_box
                        ))

            except KeyError:
                logging.error("Failed to extract assignment from csv file!")
                raise RuntimeError("Unexpected csv file entry...")
                f.write("<td markdown=\"span\" colspan=\"4\">N/A</td>\n")
        f.write("</tr>\n")

# %%
if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)

    source_root = find_sources_root()
    date_convert_path = os.path.join(source_root, '_data', 'date_convert.yml')
    assignments_path = os.path.join(source_root, '_data', 'assignments.csv')
    schedule_path = os.path.join(source_root, '_data', 'schedule.yml')
    output_path = os.path.join(source_root, 'schedule.md')

    # schedule is a list of dictionnaries.
    schedule = []
    with open(date_convert_path, 'r') as f:
        schedule = yaml.full_load(f)

    with open(assignments_path, 'r') as f:
        reader = csv.DictReader(f)

        list_of_dicts = [x for x in reader if x['name'][0] != '#']
        schedule.extend(list_of_dicts)

    with open(schedule_path, 'r') as f:
        classes = yaml.full_load(f)

    logging.info("There are {} calendar entries between assignments and class sessions"
                 .format(len(schedule)))

    schedule = sorted(schedule, key=lambda k: k['date'])

    starting_week = determine_starting_week(schedule)
    logging.info("Starting week is {}".format(starting_week))
    num_of_sessions_per_week = count_weeks(schedule, starting_week)

    logging.info("Starting to write schedule to {}...".format(output_path))
    with open(output_path, 'w') as f:
        write_schedule_header(f)
        write_table_header(f)

        # >> begin table body
        f.write("<tbody>\n")

        # main work starts here
        write_table_body(f, schedule, num_of_sessions_per_week, classes)

        # << close off table body
        f.write("</tbody>\n")

        # << close off the table
        f.write("</table>\n")

    logging.info("Done....")


# %%
