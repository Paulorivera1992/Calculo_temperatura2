LIB = -lm -ltiff

all: calculo_Tf

calculo_Tf: calculo_Tf.c
	gcc calculo_Tf.c -o calculo_Tf $(LIB)

clean:
	rm -f calculo_Tf

