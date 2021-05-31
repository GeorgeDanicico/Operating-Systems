BEGIN{
	suma = 0
}

{
if(NR > 1){
	suma=suma+$2
}

}

END{
 	print(suma/(NR-1))
}
