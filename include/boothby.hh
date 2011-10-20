#ifndef _BOOTHBY_HH_
#define _BOOTHBY_HH_ FOO

#define String std::string


void init_screen();
void uninit_screen();
void update_screen();
int get_screen_x();
int get_screen_y();

/* retrieve attribute fields */
#define ATTR_BG(attr)              ((attr) & 0x07)
#define ATTR_FG(attr)              (((attr) & 0x70) >> 4)

/* retrieve 'extended' color codes (see above for info) */
#define ATTR_XBG(attr)             ((attr) & 0x0F)
#define ATTR_XFG(attr)             (((attr) & 0xF0) >> 4)

/* set attribute fields. This requires attr to be an lvalue, and it will
 * be evaluated more than once. Use with care. */
#define ATTR_MOD_BG(attr, newbg)    attr &= 0xF8, attr |= (newbg)
#define ATTR_MOD_FG(attr, newfg)    attr &= 0x8F, attr |= ((newfg) << 4)
#define ATTR_MOD_XBG(attr, newxbg)  attr &= 0xF0, attr |= (newxbg)
#define ATTR_MOD_XFG(attr, newxfg)  attr &= 0x0F, attr |= ((newxfg) << 4)
#define ATTR_MOD_BOLD(attr, boldbit) \
                               attr &= 0x7F, attr |= (boldbit)?0x80:0x00
#define ATTR_MOD_BLINK(attr, blinkbit) \
                               attr &= 0xF7, attr |= (blinkbit)?0x08:0x00

/* these return non-zero for 'yes', zero for 'no'. Don't rely on them being 
 * any more specific than that (e.g. being exactly 1 for 'yes' or whatever). */
#define ATTR_BOLD(attr)            ((attr) & 0x80)
#define ATTR_BLINK(attr)           ((attr) & 0x08)


#endif
