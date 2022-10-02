PROJECT=tense-conds
LD65_FLAGS=
CA65_FLAGS=
EMULATOR=wslview # open with prefered app from WSL
FAMITRACKER=/mnt/c/NESDev/famitracker/FamiTracker.exe

TARGET=${PROJECT}.nes
DEBUG_FILE=${PROJECT}.dbg

.PHONY: debug run usage usage-server test FORCE

default: ${TARGET}

debug: LD65_FLAGS += --dbgfile ${DEBUG_FILE}
debug: CA65_FLAGS += -g -DDEBUG=1
debug: ${TARGET}

${TARGET}: nes.cfg \
           src/main.o \
           src/lib/neslib.o \
           src/lib/nesdoug.o \
           src/lib/unrle.o \
           src/ggsound/ggsound.o \
           src/ggsound/ggsound-api.o \
           src/crt0.o \
           src/lib/unrom.o \
           src/lib/donut.o \
           src/globals.o \
           src/attributes.o \
           src/subrand.o \
           src/gamestate.o \
           src/title-state.o \
           src/level-state.o \
           src/game-over-state.o \
           src/music/soundtrack.o \
           src/chr-data.o \
           src/palettes.o \
           src/nametables.o \
           src/map.o \
           src/metasprites.o \
           src/metatiles.o \
           src/conditions.o \
           src/player.o \
           src/enemies.o \
           src/enemies-optimizations.o \
           src/levels.o
	ld65 -C $^ nes.lib -m map.txt -o ${TARGET} ${LD65_FLAGS}

src/%.o: src/%.s
	ca65 $< ${CA65_FLAGS}

src/lib/neslib.o: src/lib/neslib.s \
                  src/lib/neslib.inc \
                  src/lib/constants.inc \
                  src/ggsound/ggsound.inc \
                  src/lib/unrom.inc
	ca65 $< ${CA65_FLAGS}

src/lib/nesdoug.o: src/lib/nesdoug.s \
                   src/lib/nesdoug.inc \
                   src/lib/constants.inc \
                   src/lib/neslib.inc
	ca65 $< ${CA65_FLAGS}

src/main.s: src/main.c \
            src/lib/nesdoug.h \
            src/lib/neslib.h \
            src/ggsound/ggsound-api.h \
	    src/music/soundtrack.h \
            src/level-state.h \
            src/gamestate.h
	cc65 -Oirs $< --add-source ${CA65_FLAGS}

src/crt0.o: src/crt0.s src/lib/neslib.s src/lib/nesdoug.s
	ca65 $< ${CA65_FLAGS}

src/ggsound/ggsound.o: src/ggsound/ggsound.s src/ggsound/ggsound.inc
	ca65 $< ${CA65_FLAGS}

src/ggsound/ggsound-api.o: src/ggsound/ggsound-api.s src/ggsound/ggsound.inc
	ca65 $< ${CA65_FLAGS}

src/globals.s: src/globals.c
	cc65 -Oirs $< --add-source ${CA65_FLAGS}

src/attributes.s: src/attributes.c src/attributes.h src/globals.h src/lib/nesdoug.h
	cc65 -Oirs $< --add-source ${CA65_FLAGS}

src/gamestate.s: src/gamestate.c \
            src/gamestate.h \
            src/lib/nesdoug.h \
            src/title-state.h \
            src/level-state.h
	cc65 -Oirs $< --add-source ${CA65_FLAGS}

src/title-state.s: src/title-state.c \
            src/title-state.h \
            src/gamestate.h \
            src/lib/neslib.h \
            src/lib/nesdoug.h \
            src/metasprites.h \
            src/palettes.h \
            src/nametables.h \
            src/globals.h \
            src/ggsound/ggsound-api.h \
            src/music/soundtrack.h
	cc65 -Oirs $< --add-source ${CA65_FLAGS}

src/level-state.s: src/level-state.c \
            src/level-state.h \
            src/gamestate.h \
            src/lib/neslib.h \
            src/lib/nesdoug.h \
            src/lib/donut.h \
            src/lib/unrle.h \
            src/ggsound/ggsound-api.h \
            src/music/soundtrack.h \
            src/subrand.h \
            src/metatiles.h \
            src/chr-data.h \
            src/palettes.h \
            src/nametables.h \
            src/attributes.h \
            src/conditions.h \
            src/enemies.h \
            src/levels.h \
            src/map.h \
            src/globals.h
	cc65 -Oirs $< --add-source ${CA65_FLAGS}

src/game-over-state.s: src/game-over-state.c \
            src/game-over-state.h \
            src/gamestate.h \
            src/lib/neslib.h \
            src/lib/nesdoug.h \
            src/lib/donut.h \
            src/ggsound/ggsound-api.h \
            src/music/soundtrack.h \
            src/chr-data.h \
            src/globals.h \
            src/palettes.h \
            src/nametables.h
	cc65 -Oirs $< --add-source ${CA65_FLAGS}

src/conditions.s: src/conditions.c \
                  src/conditions.h \
                  src/lib/nesdoug.h \
                  src/lib/neslib.h \
                  src/attributes.h \
                  src/enemies.h \
                  src/globals.h \
                  src/map.h \
                  src/metatiles.h \
                  src/subrand.h
	cc65 -Oirs $< --add-source ${CA65_FLAGS}

src/enemies.s: src/enemies.c \
               src/enemies.h \
               src/enemies-optimizations.h \
               src/lib/neslib.h \
               src/lib/nesdoug.h \
               src/globals.h \
               src/map.h \
               src/metasprites.h \
               src/player.h \
               src/subrand.h
	cc65 -Oirs $< --add-source ${CA65_FLAGS}

src/map.s: src/map.c \
           src/map.h \
           src/globals.h \
           src/map.h \
           src/metatiles.h
	cc65 -Oirs $< --add-source ${CA65_FLAGS}

src/player.s: src/player.c \
              src/player.h \
              src/lib/nesdoug.h \
              src/lib/neslib.h \
              src/conditions.h \
              src/directions.h \
              src/gamestate.h \
              src/globals.h \
              src/map.h \
              src/metasprites.h
	cc65 -Oirs $< --add-source ${CA65_FLAGS}

src/nametables.o: src/nametables.s \
                  assets/level-screen.nam.donut \
                  assets/title-screen.nam.donut \
                  assets/game-over-screen.nam.donut
	ca65 $< ${CA65_FLAGS}

src/palettes.o: src/palettes.s src/palettes.h $(wildcard assets/*.pal)
	ca65 $< ${CA65_FLAGS}

src/chr-data.o: src/chr-data.s src/chr-data.h \
                assets/gameplay-bg.chr.donut \
                assets/gameplay-spr.chr.donut
	ca65 $< ${CA65_FLAGS}

src/music/soundtrack.o: src/music/soundtrack.s src/music/soundtrack.asm
	ca65 $< ${CA65_FLAGS}

src/music/soundtrack.asm: src/music/soundtrack.txt
	python tools/ft_txt_to_asm.py $<

src/music/soundtrack.txt: src/music/soundtrack.ftm
	${FAMITRACKER} $^ -export $@

src/levels.s: tools/compile-levels.rb assets/tiled $(wildcard assets/tiled/*)
	ruby tools/compile-levels.rb assets/tiled $@

%.donut: % tools/donut
	tools/donut -f $< -o $@

clean:
	bash -c 'for f in src/*.s src/*/*.s; do [[ -e ${f%.s}.c ]] && rm "$f"; done'; \
        find . -name '*.o' -exec rm '{}' \; ; \
        find . -name '*.rle' -exec rm '{}' \; ; \
	rm src/music/soundtrack.txt \
           src/music/soundtrack.asm \
           src/music/soundtrack_dpcm.asm \
           ${TARGET} ${DEBUG_FILE} map.txt \
           tools/ld65-map.json \
           -f

run: debug
	${EMULATOR} ${TARGET} >/dev/null 2>/dev/null

usage: tools/ld65-map.json

tools/ld65-map.json: nes.cfg tools/ld65-map.rb debug
	ruby tools/ld65-map.rb map.txt nes.cfg tools/ld65-map.json

tools/donut: tools/donut.c
	gcc $^ -o $@

usage-server: usage
	python3 -m http.server 8000 --directory tools/ &

FORCE:
