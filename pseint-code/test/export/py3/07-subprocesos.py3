def funciondoble(b):
	a = 2*b
	return a
def noretornanada(a, b):
	print(a+b)
def porreferencia(b):
	b = 7
def recibevector(v):
	v[0] = input()
def recibematriz(m):
	m[0][0] = input()
if __name__ == '__main__':
	print(funciondoble(5))
	noretornanada(3,9)
	c = 0
	porreferencia(c)
	print(c)
	a = [str() for ind0 in range(10)]
	b = [[str() for ind0 in range(4)] for ind1 in range(3)]
	recibevector(a)
	recibematriz(b)
