#ifndef __IMAGES_H__
#define __IMAGES_H__

#ifdef WIN32

#define DEFIMAGE(image) \
    extern char binary_ ## image ## _bmp_start; \
    extern char binary_ ## image ## _bmp_end;

#define IMAGE_BUF(image)    (&binary_ ## image ## _bmp_start)
#define IMAGE_SIZE(image)   (&binary_ ## image ## _bmp_end - &binary_ ## image ## _bmp_start)

#else /* ! WIN32 */

#define DEFIMAGE(image) \
    extern char _binary_ ## image ## _bmp_start; \
    extern char _binary_ ## image ## _bmp_end;

#define IMAGE_BUF(image)    (&_binary_ ## image ## _bmp_start)
#define IMAGE_SIZE(image)   (&_binary_ ## image ## _bmp_end - &_binary_ ## image ## _bmp_start)

#endif /* WIN32 */

DEFIMAGE(buttonbar)
DEFIMAGE(buttonbar2)
DEFIMAGE(enemy1)
DEFIMAGE(enemy2)
DEFIMAGE(enemy3)
DEFIMAGE(leveltiles)
DEFIMAGE(menuscreen)
DEFIMAGE(minipause)
DEFIMAGE(tower1_2)
DEFIMAGE(tower1_3)
DEFIMAGE(tower1)
DEFIMAGE(tower2_2)
DEFIMAGE(tower2_3)
DEFIMAGE(tower2)
DEFIMAGE(tower3_2)
DEFIMAGE(tower3_3)
DEFIMAGE(tower3)
DEFIMAGE(tower4)
DEFIMAGE(tut_basic)
DEFIMAGE(tut_upgrading)

#undef DEFIMAGE

#endif /* __IMAGES_H__ */
