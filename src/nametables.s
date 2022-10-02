.segment "RODATA"

.export _level_screen
.export _title_screen
.export _game_over_screen

_title_screen: .incbin "../assets/title-screen.nam.donut"
_level_screen: .incbin "../assets/level-screen.nam.donut"
_game_over_screen: .incbin "../assets/game-over-screen.nam.donut"
