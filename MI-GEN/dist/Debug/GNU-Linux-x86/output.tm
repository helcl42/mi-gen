* TinyMachine Code:
  0:     LD  6,0(0) 
  1:     ST  0,0(0) 
  2:    LDC  0,1(0) 
  3:     ST  0,0(5) 
  4:     LD  0,0(5) 
  5:    OUT  0,0,0 
  6:    LDC  0,4(0) 
  7:     ST  0,2(5) 
  8:    LDC  0,4(0) 
  9:     ST  0,1(5) 
 10:     LD  0,2(5) 
 11:    OUT  0,0,0 
 12:    LDC  0,45(0) 
 13:     ST  0,2(5) 
 14:     LD  0,1(5) 
 15:     ST  0,0(6) 
 16:    LDC  0,1(0) 
 17:     LD  1,0(6) 
 18:    ADD  0,1,0 
 19:     ST  0,1(5) 
 20:     LD  0,1(5) 
 21:    OUT  0,0,0 
 22:    LDC  0,2(0) 
 23:     ST  0,0(5) 
 24:     LD  0,1(5) 
 25:     ST  0,0(6) 
 26:    LDC  0,14(0) 
 27:     LD  1,0(6) 
 28:    SUB  0,1,0 
 29:    JLT  0,2(7) 
 30:    LDC  0,0(0) 
 31:    LDA  7,1(7) 
 32:    LDC  0,1(0) 
 34:     LD  0,1(5) 
 35:     ST  0,0(6) 
 36:    LDC  0,1(0) 
 37:     LD  1,0(6) 
 38:    ADD  0,1,0 
 39:     ST  0,1(5) 
 40:     LD  0,1(5) 
 41:    OUT  0,0,0 
 42:    LDA  7,-19(7) 
 33:    JEQ  0,9(7) 
 43:     LD  0,0(5) 
 44:    OUT  0,0,0 
 45:     LD  0,2(5) 
 46:    OUT  0,0,0 
 47:   HALT  0,0,0 