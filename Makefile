#
# Last updated: Wed 22 Nov 2023
#

# configuration options, adjust as you see fit
SHELL=bash
TERM=2324b
USERNAME=noureddi
REMOTE_DIR=addiator.rose-hulman.edu:/class/csse/csse332/${TERM}

all: serve

.PHONY: clean all

serve:
	bundle exec jekyll serve -q -l -o

background:
	tmux new -d -s server "bundle exec jekyll serve -q -l -o"

restart: kill background

kill:
	tmux kill-session -t server || true

www: clean
	@echo -e '\033[1;mBuilding the webpage\033[0m'
	bundle exec jekyll build -q
	@echo -e '\033[1;32mCopying to remote server\033[0m'

echo:
	@echo -e 'TERM is ${TERM}'
	@echo -e 'USERNAME is ${USERNAME}'
	@echo -e 'Remote target is ${REMOTE_DIR}'

clean: kill
	bundle exec jekyll clean

