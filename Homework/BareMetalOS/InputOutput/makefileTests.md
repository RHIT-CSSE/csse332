# Test 1

### Given

* remove all binaries, including floppya.img, bootload and kernel.

### When

run "make"

### Then

Everything should build correctly

### Common mistakes

* not compiling bootload
* not zeroing floppya.img and load bootload into it.

# Test 2

### Given
	the program runs well

### When

modify kernel.c a little bit, and run "make"

### Then

* only kernel is recompiled (but not other .os)
* everything is relinked
* floppya.img is reinitialized from scratch using /dev/zero
* kernel is loaded into floppya.img

### Common mistakes

* kernel is not in the dependency list of floppya.img
* kernel just rewritten over existing floppya.img


# Test 3

### Given
	
the program runs well

### When
	
remove floppya.img

### Then

floppya.img should be rebuilt, nothing should be recompiled/assembled/linked, and the program should still runs

### Common Mistakes

The rule that generates floppya.img is not named floopya.img, so makefile does not recognize the changes.


