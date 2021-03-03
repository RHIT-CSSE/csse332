# %%
import yaml
import pprint as pp
import csv
import logging
import os


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


# %%
