#ifndef MyGraphics
    #define MyGraphics

    /* Stroke font constants (use these in GLUT program). */
    #define GLUT_STROKE_ROMAN		((void*)0)
    #define GLUT_STROKE_MONO_ROMAN		((void*)1)

    /* Bitmap font constants (use these in GLUT program). */
    #define GLUT_BITMAP_9_BY_15		((void*)2)
    #define GLUT_BITMAP_8_BY_13		((void*)3)
    #define GLUT_BITMAP_TIMES_ROMAN_10	((void*)4)
    #define GLUT_BITMAP_TIMES_ROMAN_24	((void*)5)
    //#if (GLUT_API_VERSION >= 3)
    #define GLUT_BITMAP_HELVETICA_10	((void*)6)
    #define GLUT_BITMAP_HELVETICA_12	((void*)7)
    #define GLUT_BITMAP_HELVETICA_18	((void*)8)


    /* Mouse buttons. */
    #define GLUT_LEFT_BUTTON		0
    #define GLUT_MIDDLE_BUTTON		1
    #define GLUT_RIGHT_BUTTON		2

    /* Mouse button  state. */
    #define GLUT_DOWN			0
    #define GLUT_UP				1

    //#if (GLUT_API_VERSION >= 2)
    /* function keys */
    #define GLUT_KEY_F1			1
    #define GLUT_KEY_F2			2
    #define GLUT_KEY_F3			3
    #define GLUT_KEY_F4			4
    #define GLUT_KEY_F5			5
    #define GLUT_KEY_F6			6
    #define GLUT_KEY_F7			7
    #define GLUT_KEY_F8			8
    #define GLUT_KEY_F9			9
    #define GLUT_KEY_F10			10
    #define GLUT_KEY_F11			11
    #define GLUT_KEY_F12			12
    /* directional keys */
    #define GLUT_KEY_LEFT			100
    #define GLUT_KEY_UP			101
    #define GLUT_KEY_RIGHT			102
    #define GLUT_KEY_DOWN			103
    #define GLUT_KEY_PAGE_UP		104
    #define GLUT_KEY_PAGE_DOWN		105
    #define GLUT_KEY_HOME			106
    #define GLUT_KEY_END			107
    #define GLUT_KEY_INSERT			108
    //#endif

    /* Entry/exit  state. */
    #define GLUT_LEFT			0
    #define GLUT_ENTERED			1

    /* Menu usage  state. */
    #define GLUT_MENU_NOT_IN_USE		0
    #define GLUT_MENU_IN_USE		1

    /* Visibility  state. */
    #define GLUT_NOT_VISIBLE		0
    #define GLUT_VISIBLE			1

    /* Window status  state. */
    #define GLUT_HIDDEN			0
    #define GLUT_FULLY_RETAINED		1
    #define GLUT_PARTIALLY_RETAINED		2
    #define GLUT_FULLY_COVERED		3

    /* Color index component selection values. */
    #define GLUT_RED			0
    #define GLUT_GREEN			1
    #define GLUT_BLUE			2

    void iSetColor(double r, double g, double b);
    void iPoint(double x, double y, int size);
    void iLine(double x1, double y1, double x2, double y2);
    void iFilledPolygon(double x[], double y[], int n);
    void iPolygon(double x[], double y[], int n);
    void iRectangle(double left, double bottom, double dx, double dy);
    void iFilledRectangle(double left, double bottom, double dx, double dy);
    void iFilledCircle(double x, double y, double r, int slices = 100);
    void iCircle(double x, double y, double r, int slices = 100);
    void iEllipse(double x, double y, double a, double b, int slices = 100);
    void iFilledEllipse(double x, double y, double a, double b, int slices = 100);
    void iText(double x, double y,char *str, void* font=GLUT_BITMAP_8_BY_13);
    void iSetTimer(int msec, void(*f)(void));
    void iPauseTimer(int index);
    void iResumeTimer(int index);
    void iShowBMP(int x, int y, char filename[]);
    void iShowBMP2(int x, int y, char filename[], int ignorecolour);

#endif // MyGraphics
