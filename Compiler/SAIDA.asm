		@ /0000 ;
programa		SC	main	;
zero			K	=0		;
one				K	=1		;
two				K	=2		;
ten				K 	=10		;
sixteen			K	=16		;
STOP			K   /0FF0	;

FFFF			K	/FFFF	    ; Base de representação

ascii_offset	K	/30			; Offset para o código de um número na tabela ASCII
ascii_minus		K	/2D			; Sinal de menos em ASCII
ascii_cr		K	/D			; Carriage return em ASCII
ascii_lf		K	/A			; Line feed em ASCII


output_number	K 	=0			; Número para ser impresso
o_temp1			K	=0			; Guarda o valor da última dezena
o_temp2			K	=1			; Indicador da dezena

output		JP	/0000				;
			LD	ten					; Inicialização
			MM	o_temp2				;
			LD	output_number		; 
			JN	o_negative			; Número negativo
			JP  o_start				; Número posítivo
o_negative	LD	ascii_minus			; Caso contrário imprime "-"
			PD	/0100				;
			LD	FFFF				; E inverte o número
			-	output_number		;
			+	one					;
			MM	output_number		;
o_start		MM	o_temp1				;
o_loop		LD	output_number		; Carrega o número
			/	o_temp2				;
			JZ	o_print				; Imprime se é o número mais a esquerda.
			MM	o_temp1				; Se não, guarda o número 
			LD	o_temp2				; Aumenta a casa decimal
			*	ten					;
			MM	o_temp2				;
			JP	o_loop				; E volta para o loop
o_print		LD	o_temp1				; Impressão do número
			+	ascii_offset		;
			PD	/0100				;
			LD	o_temp2				; Verifica se é o último número
			/	ten					;
			MM	o_temp2				;
			-	one					;
			JZ	o_end				; Vai para o final, se ímprimiu tudo
			LD	o_temp1				;
			*	o_temp2				;
			MM	o_temp1				;
			LD	output_number		; Atualiza o número para impressão
			-	o_temp1				;
			MM	output_number		;
			MM	o_temp1				;
			LD	ten					;
			MM	o_temp2				;
			JP	o_loop				; Imprime o próximo caracter

o_end		RS	output				; Final da rotina


input_number	K	=0				; Variável de retorno da rotina
i_negative		K	=0				; Número digitado é negativo
i_temp			K	=0				; Variáveis temporárias
i_temp2			K	=0				;

input		JP	/0000				;
			LD  zero				; Inicialização
			MM	input_number		;
			MM	i_negative			;
			MM	i_temp				;
			MM	i_temp2				;
			GD	/0000				; Leitura de número negativo
			MM	i_temp				; Guarda caracteres lidos em i_temp
			/	sixteen				; Obtém o primeiro caracter
			/	sixteen				; 
			MM	i_temp2				;
			-	ascii_minus			; Verifica se número digitado é negativo
			JZ	set_i_negative		;
			JP	1st_char			; Não é negativo, lê como número
set_i_negative	LD	one				; Carrega o i_negative com FFFF
			MM	i_negative			;
			JP	2nd_char			; Lê o segundo caracter
i_loop		GD	/0000				; Loop de leitura
			MM	i_temp				; Guarda caracteres lidos em i_temp
			/	sixteen				; Obtém o primeiro caracter
			/	sixteen				; 
			MM	i_temp2				;
1st_char	-	ascii_cr			; Verifica se é o fim (ascii \d ou \a)
			JZ	i_end				; 
			LD	i_temp2				;
			- 	ascii_lf			;
			JZ	i_end				;
			LD	input_number		; Não é o último caracter
			*	ten					; Aumenta uma dezena no resultado
			MM	input_number		;	
			LD	i_temp2				; Converte caracter lido em número
			-	ascii_offset		;
			+	input_number		; Soma no resultado de retorno
			MM	input_number		; Atualiza o resultado de retorno
2nd_char	LD	i_temp2				; Obtém o segundo caracter
			*	sixteen				;
			*	sixteen				;
			MM	i_temp2				;
			LD	i_temp				;
			-	i_temp2				;
			MM	i_temp2				;
			-	ascii_cr			; Verifica se é o fim (ascii \d ou \a)
			JZ	i_end				; 
			LD	i_temp2				;
			- 	ascii_lf			;
			JZ	i_end				;
			LD	input_number		; Não é o último caracter
			*	ten					; Aumenta uma dezena no resultado
			MM	input_number		;	
			LD	i_temp2				; Converte caracter lido em número
			-	ascii_offset		;
			+	input_number		; Soma no resultado de retorno
			MM	input_number		; Atualiza o resultado de retorno
			JP	i_loop				; Lê o proximo caracter
i_end		LD	i_negative			; Transforma em negativo se negativo
			JZ  i_return			;
			LD	zero				;
			-	input_number		;
			MM	input_number		;
i_return	LD	input_number		;
			RS	input				;

load_inst       LD  /0000				; Instrução para o acesso indireto
store_inst		MM	/0000				; Instrução para store indireto
return_inst		RS	/0000				; Instrução para retorno indireto
pos_param       K   =0					; Posição do parâmetro da função
                                      
load_ra_pos  	JP  /0000               ; Ponto de entrada da subrotina
                LD  STOP                ; Carrega topo da pilha do R.A.
                -   two       			; Diminui um endereço na pilha do R.A.
                -   pos_param			; Accumulador com o endereço correto
                +   load_inst  			; Here's the magic: Cria instrução nova!
                MM  hack				; Armazena como a PROXIMA INSTRUCAO! 
hack			K  /0                   ; Reservado para guardar a instrução recém-montada
                RS load_ra_pos		    ; Thanks to Débora for this piece of gold

store_ra_pos	JP	/0000				;
				LD  STOP                ; Carrega topo da pilha do R.A.
                -   two       			; Diminui um endereço na pilha do R.A.
                -   pos_param			; Accumulador com o endereço correto
                +   store_inst  		; Here's the magic: Cria instrução nova!
                MM  hack2				; Armazena como a PROXIMA INSTRUCAO! 
hack2			K  /0                   ; Reservado para guardar a instrução recém-montada
                RS store_ra_pos		    ;

ra_tam			K   =0			;
ra_end			K	=0400		;

cria_ra			JP  /0000		;
				LD	STOP		;
				+	two			;
				MM	STOP		;
				LD	zero		;
				MM	pos_param    ;
				LD	ra_end		 ;
				SC	store_ra_pos ;
				LD  STOP		;
				+	ra_tam		;
				MM	STOP		;
				RS	cria_ra		;

				@ /0200


main		JP  /0000		;
			SC  input	; Comando de input
			MM  V0		;
			LD  V0				; Comando de output
			MM  output_number	;
			SC  output			;
I0			LD  zero	; Begin if case
			LD  K0		; Comparacao X < Y
			-  V0		;
			MM  T0		;
			LD  T0		;
			JN  _I0		;
			JZ  _I0		;
			LD  K1		; Atribuicao de variavel
			MM  V1		;
			JP  E0		;
_I0			LD  zero	; End if case/Begin else case
			LD  K2		; Atribuicao de variavel
			MM  V1		;
L0			LD  zero	; Begin while loop
			LD  V0		; Comparacao X > Y
			-   K2		;
			MM  T1		;
			LD  T1		;
			JN  _L0		;
			JZ  _L0		;
			LD  V1		;
			*   V0		;
			MM  T2		;
			LD  T2		; Atribuicao de variavel
			MM  V1		;
			LD  V0		;
			-   K3		;
			MM  T3		;
			LD  T3		; Atribuicao de variavel
			MM  V0		;
			JP  L0		;
_L0			LD  zero	; End while loop
E0			LD  zero	; End else case
			LD  V1				; Comando de output
			MM  output_number	;
			SC  output			;
			HM  /00		;
			#  P 		;

		 @ /0A00
V0			K  =0		; Declaracao de variavel
K0			K  =0		; Declaracao de constante
T0			K  =0		; Declaracao de temporario
V1			K  =0		; Declaracao de variavel
K1			K  =0		; Declaracao de constante
K2			K  =1		; Declaracao de constante
T1			K  =0		; Declaracao de temporario
T2			K  =0		; Declaracao de temporario
K3			K  =1		; Declaracao de constante
T3			K  =0		; Declaracao de temporario
