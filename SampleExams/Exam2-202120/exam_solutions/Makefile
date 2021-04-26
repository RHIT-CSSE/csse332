all: leader_follower fruit_eater wolf_pig print1n2

leader_follower: leader_follower.c
	gcc -pthread -ggdb leader_follower.c -o leader_follower

fruit_eater: fruit_eater.c
	gcc -pthread -ggdb fruit_eater.c -o fruit_eater

wolf_pig: wolf_pig.c
	gcc -pthread -ggdb wolf_pig.c -o wolf_pig

print1n2: print1n2.c
	gcc -pthread -ggdb print1n2.c -o print1n2

clean:
	rm *.txt leader_follower fruit_eater wolf_pig print1n2
