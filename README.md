# compress
Trabalho da disciplina INE5410 - Programação Concorrente. 

O trabalho consiste em paralelizar o software um software de compressão que utiliza o algoritmo Lempel-Ziv-Welch usando a biblioteca pthread. 

O Lempel-Ziv-Welch (LZW) é um algoritmo de compressão de dados sem perdas proposto por Abraham Lempel,
Jacob Ziv e Terry Welch em 1984, que encontra aplicações em ferramentas de compressão do sistema Unix e na
compactação de imagens nos formatos GIF e TIFF. A ideia principal do algoritmo consiste em manter um dicionário
com códigos para prefixos da entrada e usar esses códigos para representar os dados originais. Para a compressão de
dados, inicialmente, o dicionário é inicializado com todos os símbolos da cadeia de entrada. No caso de codificações de
bytes seriam todos os números de 0 a 256. Em seguida, a entrada é lida e acumulada em uma variável s. Sempre que a
sequência s não estiver presente no dicionário, um novo código, correspondente à versão anterior de s (isto é, s sem o
último símbolo lido), é emitido e s é adicionado ao dicionário. Então, s é inicializado novamente com o último símbolo
lido e o processo continua até que não haja mais símbolos a serem lidos na entrada. No caso da descompressão, o
processo inverso é feito.
