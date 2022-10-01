.segment "RODATA"

.export _level_screen
.export _title_screen

_title_screen: .incbin "../assets/title-screen.nam.donut"
_level_screen: .incbin "../assets/level-screen.nam.donut"
