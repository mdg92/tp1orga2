#include "tdt.h"

void tdt_agregar(tdt* tabla, uint8_t* clave, uint8_t* valor) {
	valorValido v;
	v.valido=1;
	for (int i=0;i<15;i++){
		v.valor.val[i]=*(valor+i);
	}
	if(tabla->primera!=0){
		if (tabla->primera->entradas[*clave]!=0){
			if (tabla->primera->entradas[*clave]->entradas[*(clave+1)]!=0){
				tabla->primera->entradas[*clave]->entradas[*(clave+1)]->entradas[*(clave+2)]=v;
			}else{
				tabla->primera->entradas[*clave]->entradas[*(clave+1)]=malloc(sizeof (tdtN3));
				tabla->primera->entradas[*clave]->entradas[*(clave+1)]->entradas[*(clave+2)]=v;
			}
		}else{
			tabla->primera->entradas[*clave]=malloc(sizeof (tdtN2));
			tabla->primera->entradas[*clave]->entradas[*(clave+1)]=malloc(sizeof (tdtN3));
			tabla->primera->entradas[*clave]->entradas[*(clave+1)]->entradas[*(clave+2)]=v;
		}
	}else{
		tabla->primera=malloc(sizeof(tdtN1));
		tabla->primera->entradas[*clave]=malloc(sizeof (tdtN2));
		tabla->primera->entradas[*clave]->entradas[*(clave+1)]=malloc(sizeof (tdtN3));
		tabla->primera->entradas[*clave]->entradas[*(clave+1)]->entradas[*(clave+2)]=v;
	}
	tabla->cantidad+=1;
}

void tdt_borrar(tdt* tabla, uint8_t* clave) {
	if (tabla->primera!=0){
		if (tabla->primera->entradas[*clave]!=0){
			if (tabla->primera->entradas[*clave]->entradas[*(clave+1)]!=0){
				if (tabla->primera->entradas[*clave]->entradas[*(clave+1)]->entradas[*(clave+2)].valido!=0){
					tabla->primera->entradas[*clave]->entradas[*(clave+1)]->entradas[*(clave+2)].valido=0;
					int i=0;
					while (i<256&&(tabla->primera->entradas[*clave]->entradas[*(clave+1)]->entradas[i].valido==0)) i++;
					if (i==256){
						free (tabla->primera->entradas[*clave]->entradas[*(clave+1)]);
						i=0;
						while (i<256&&(tabla->primera->entradas[*clave]->entradas[i]==0)) i++;
						if (i==256){
							free(tabla->primera->entradas[*clave]);
							i=0;
							while (i<256&&(tabla->primera->entradas[i]==0))	i++;
							if (i==256) free(tabla->primera);
						}		
					}
					tabla->cantidad-=1;
				}
			}
		}
	}
}

void tdt_imprimirTraducciones(tdt* tabla, FILE *pFile) {
	fprintf(pFile, "- %s - \n", tabla->identificacion);
	if (tabla->primera!=0){
		int i=0;
		while (i<256){
			if (tabla->primera->entradas[i]!=0){
				int j=0;
				while(j<256){
					if (tabla->primera->entradas[i]->entradas[j]!=0){
						int k=0;
						while (k<256){
							if (tabla->primera->entradas[i]->entradas[j]->entradas[k].valido!=0){
								fprintf(pFile, "%02X",i);
								fprintf(pFile, "%02X",j);
								fprintf(pFile, "%02X=>",k);
								fprintf(pFile, "%02X", tabla->primera->entradas[i]->entradas[j]->entradas[k].valor.val[0]);
								fprintf(pFile, "%02X", tabla->primera->entradas[i]->entradas[j]->entradas[k].valor.val[1]);
								fprintf(pFile, "%02X", tabla->primera->entradas[i]->entradas[j]->entradas[k].valor.val[2]);
								fprintf(pFile, "%02X", tabla->primera->entradas[i]->entradas[j]->entradas[k].valor.val[3]);
								fprintf(pFile, "%02X", tabla->primera->entradas[i]->entradas[j]->entradas[k].valor.val[4]);
								fprintf(pFile, "%02X", tabla->primera->entradas[i]->entradas[j]->entradas[k].valor.val[5]);
								fprintf(pFile, "%02X", tabla->primera->entradas[i]->entradas[j]->entradas[k].valor.val[6]);
								fprintf(pFile, "%02X", tabla->primera->entradas[i]->entradas[j]->entradas[k].valor.val[7]);
								fprintf(pFile, "%02X", tabla->primera->entradas[i]->entradas[j]->entradas[k].valor.val[8]);
								fprintf(pFile, "%02X", tabla->primera->entradas[i]->entradas[j]->entradas[k].valor.val[9]);
								fprintf(pFile, "%02X", tabla->primera->entradas[i]->entradas[j]->entradas[k].valor.val[10]);
								fprintf(pFile, "%02X", tabla->primera->entradas[i]->entradas[j]->entradas[k].valor.val[11]);
								fprintf(pFile, "%02X", tabla->primera->entradas[i]->entradas[j]->entradas[k].valor.val[12]);
								fprintf(pFile, "%02X", tabla->primera->entradas[i]->entradas[j]->entradas[k].valor.val[13]);
								fprintf(pFile, "%02X\n", tabla->primera->entradas[i]->entradas[j]->entradas[k].valor.val[14]);
							}
							k++;
						}
					}
					j++;
				}
			}
			i++;
		}
	}
}

maxmin* tdt_obtenerMaxMin(tdt* tabla) {
	maxmin* obj=malloc(sizeof(maxmin));
	uint8_t minc[3];
	uint8_t i=0;
	uint8_t j=0;
	uint8_t k=0;
	for(int u=0;u<3;u++) obj->min_clave[u]=0;
	for(int u=0;u<3;u++) obj->max_clave[u]=255;
	for(int u=0;u<15;u++) obj->min_valor[u]=255;
	for(int u=0;u<15;u++) obj->max_valor[u]=0;
	if(tabla->primera!=0){
		while (tabla->primera->entradas[i]==0) i++;
		while (tabla->primera->entradas[i]->entradas[j]==0) j++;
		while (tabla->primera->entradas[i]->entradas[j]->entradas[k].valido==0) k++;
		minc[0]=i, minc[1]=j, minc[2]=k;
		for(int u=0;u<3;u++) obj->min_clave[u]=minc[u];
		i=j=k=255;
		while ( (tabla->primera->entradas[i]==0)) i--;
		while ( (tabla->primera->entradas[i]->entradas[j]==0)) j--;
		while ( (tabla->primera->entradas[i]->entradas[j]->entradas[k].valido==0)) k--;
		minc[0]=i, minc[1]=j, minc[2]=k;
		for(int u=0;u<3;u++) obj->max_clave[u]=minc[u];
		i=j=k=0;
		int r,s,t;
		r=s=t=0;
		while(r!=1){
			s=0;
			while(s!=1&&(tabla->primera->entradas[i]!=0)){
				t=0;
				while(t!=1&&(tabla->primera->entradas[i]->entradas[j]!=0)){
					int l=0;
					if (tabla->primera->entradas[i]->entradas[j]->entradas[k].valido!=0){
						while(l<15&&(obj->max_valor[l]<tabla->primera->entradas[i]->entradas[j]->entradas[k].valor.val[l])) l++;
						if (l==15) {for(int u=0;u<15;u++) obj->max_valor[u]=tabla->primera->entradas[i]->entradas[j]->entradas[k].valor.val[u];}
						l=0;
						while(l<15&&(obj->min_valor[l]>tabla->primera->entradas[i]->entradas[j]->entradas[k].valor.val[l])) l++;
						if (l==15) {for(int u=0;u<15;u++) obj->min_valor[u]=tabla->primera->entradas[i]->entradas[j]->entradas[k].valor.val[u];}
					}
					if (k==255) t=1;
					k++;
				}
				if (j==255) s=1;
				j++;
			}
			if (i==255) r=1;
			i++;
		} 
	}
	return obj;
}
