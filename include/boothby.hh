/*
 * Copyright (C) 2011, Paul Tagliamonte <tag@pault.ag>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef _BOOTHBY_HH_
#define _BOOTHBY_HH_ FOO

#define String std::string

#define DEBUG(x) std::cerr << x << std::endl;

void init_screen();
void uninit_screen();
void update_screen();

#define SLEEPDEBUG(foo) \
	move(0,0); printw("%d", foo); refresh(); usleep(2000000)

#define ATTR_BG(attr)              ((attr) & 0x07)
#define ATTR_FG(attr)              (((attr) & 0x70) >> 4)
#define ATTR_XBG(attr)             ((attr) & 0x0F)
#define ATTR_XFG(attr)             (((attr) & 0xF0) >> 4)

#define ATTR_MOD_BG(attr, newbg)    attr &= 0xF8, attr |= (newbg)
#define ATTR_MOD_FG(attr, newfg)    attr &= 0x8F, attr |= ((newfg) << 4)
#define ATTR_MOD_XBG(attr, newxbg)  attr &= 0xF0, attr |= (newxbg)
#define ATTR_MOD_XFG(attr, newxfg)  attr &= 0x0F, attr |= ((newxfg) << 4)

#define ATTR_MOD_BOLD(attr, boldbit) \
                               attr &= 0x7F, attr |= (boldbit)?0x80:0x00
#define ATTR_MOD_BLINK(attr, blinkbit) \
                               attr &= 0xF7, attr |= (blinkbit)?0x08:0x00
#define ATTR_BOLD(attr)            ((attr) & 0x80)
#define ATTR_BLINK(attr)           ((attr) & 0x08)

#endif
