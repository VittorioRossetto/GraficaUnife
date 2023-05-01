main: B-spline/main.c SuperficieLuminosa/main.c CurvaInterpolante/main.c
	gcc -o BSpline.o B-spline/main.c -lGL -lGLU -lglut -lGLEW -lSOIL -lm
	gcc -o CurvaInterpolante.o CurvaInterpolante/main.c -lGL -lGLU -lglut -lGLEW -lSOIL -lm
	gcc -o SuperficieLuminosa.o SuperficieLuminosa/main.c -lGL -lGLU -lglut -lGLEW -lSOIL -lm
	export MESA_GL_VERSION_OVERRIDE=4.5