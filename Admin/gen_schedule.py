import yaml
import pprint as pp
import csv

if __name__ == '__main__':
    with open(r'./_data/date_convert.yml') as f:
        schedule = yaml.full_load(f)

        pp.pprint(schedule[0]['date'])

    with open(r'./_data/assignments.csv') as f:
        reader = csv.DictReader(f)

        list_of_dicts = [x for x in reader if x['name'][0] != '#']
        pp.pprint(list_of_dicts)