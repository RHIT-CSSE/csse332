all: serve

.PHONY: all

serve:
	bundle exec jekyll serve -q -l -o

background:
	tmux new -d -s server "bundle exec jekyll serve -q -l -o"

restart: kill background

kill:
	tmux kill-session -t server || true

clean: kill
	bundle exec jekyll clean

