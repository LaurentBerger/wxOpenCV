//
//
// ************* Constantes pour le type du pixel
//					TYPE DU PIXEL
//

//		ATTRIBUTS
#define NON_SIGNE_IMAGE				0
#define SIGNE_IMAGE					1
#define BINARISE_IMAGE				2
#define SEGMENTE_IMAGE				4
#define IMAGECOULEUR_IMAGE			8
#define DISTANCE_IMAGE				16
#define ATTRIBUTINCONNU_IMAGE		0xE0L


//		TYPE
#define ENTIER_IMAGE				256
#define REEL_IMAGE					512
#define CPLX_IMAGE					1024
#define TABLEAU_IMAGE				2048
#define TABLEAU_CLASSE				4096
#define TYPEPIXELINCONNU_IMAGE 		0XF800L

#define PTR_IMAGE_CHAR				1
#define PTR_IMAGE_UNSIGNEDCHAR		2
#define PTR_IMAGE_SHORT				3
#define PTR_IMAGE_UNSIGNEDSHORT		4
#define PTR_IMAGE_LONG				5
#define PTR_IMAGE_UNSIGNEDLONG		6
#define PTR_IMAGE_FLOAT				7
#define PTR_IMAGE_DOUBLE			8
#define PTR_IMAGE_CPLXFLOAT			9
#define PTR_IMAGE_CPLXDOUBLE		10

// Fonction Image
#define FCT_PERIODE					1


//
//
// ************* Constantes pour la taille du pixel
//					TAILLE DU PIXEL
//
//

#define T_BINAIRE1_IMAGE		1	// ramenÈ ‡ 8 bits en mémoire
#define T_BINAIRE2_IMAGE		2	// ramenÈ ‡ 8 bits en mémoire
#define T_BINAIRE4_IMAGE		4	// ramenÈ ‡ 8 bits en mémoire
#define T_OCTET_IMAGE			8
#define T_SHORT_IMAGE			sizeof(short)*8
#define T_LONG_IMAGE			sizeof(int)*8
#define T_FLOAT_IMAGE			sizeof(float)*8
#define T_DOUBLE_IMAGE			sizeof(double)*8
#define T_CPLX_IMAGE			sizeof(float)*8*2
#define T_CPLX_DOUBLE_IMAGE		sizeof(double)*8*2
#define LOG2TAILLEMAX_IMAGE	7

// Constante pour les rÈgions
#define CONNEXITE4 2	/* Objet de connexité 4 donc connexité 8 pour le contour 	*/
#define CONNEXITE8 1	/* Objet de connexité 8 donc connexité 4 pour le contour 	*/



#define OpMAXIMUM			0x41
#define OpMINIMUM			0x42
#define OpDILATATION		0x43
#define OpEROSION			0x44
#define OpADDITION			0x45
#define OpPRODUIT			0x4A
#define OpSOUSTRACTION		0x46
#define OpCONVOLUTION		0x47
#define OpTRACEDROITE		0x48
#define OpVORONOI			0x49
#define OpINTERCOR			0x4B
#define OpDIVISION			0x4C
#define OpVARIANCE			0x4D

#define OpABS				0x01
#define OpSQR				0x02
#define OpFFT				0x03
#define OpIFFT				0x04
#define OpDONDELETTE		0x05
#define OpIONDELETTE		0x06
#define OpBINARISATION		0x07
#define OpBINARISATIONLOC	0x08
#define OpCOMPOSANTECONNEXE	0x09
#define OpDISTANCEOBJFOND	0x0A
#define OpDISTANCEFOND		0x0B
#define OpLIGNEMEDIANE		0x0C
#define OpHOUGHDROITE		0x0D
#define OpCOPIE				0x0E
#define OpSPCPUI			0x0F
#define OpAUTOCOR			0x10


