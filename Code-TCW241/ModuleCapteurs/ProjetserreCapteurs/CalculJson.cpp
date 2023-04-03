#include "CalculJson.h"

float CalculJson::quintToFloat(quint16* arr)
{
	static_assert(std::numeric_limits<float>::is_iec559, "Only supports IEC 559 (IEEE 754) float");

	quint32 temp = (arr[0] << 16) | arr[1];
	//La variable temp est un entier de 32 bits, arr est un tableau de 2 cases d'entiers de 16 bits.
	//En partant de la fin, le programme place arr[0] au bit 16 puis arr[1] au bit 0

	float* out = reinterpret_cast<float*>(&temp);
	//le compilateur va forcer la transformation d'un quint32 en float

	return *out;
}

float CalculJson::calculHumidite(float Vout) {
	float humid = -1.91 * pow(10, -9) * pow(Vout, 3);
	humid += 1.33 * pow(10, -5) * pow(Vout, 2);
	humid += 9.56 * pow(10, -3) * Vout;
	humid -= 2.16 * pow(10, 1);
	return humid;
}

QJsonValue CalculJson::valeurJson(quint16* arr, char car, bool bac)
{
	float val = quintToFloat(arr);
	QString affichage;
	switch (car) {
	case 'T':
		affichage = QString::number(double(val), 'f', 1) + 0xB0 + "C";
		break;
	case 'H':
		if (bac) val = calculHumidite(val);
		affichage = QString::number(double(val), 'f', 1) + "%";
		break;
	default:
		affichage = QString::number(double(val), 'f', 1);
		return 0;
	}
	QJsonValue valeur(affichage);
	return valeur;
}
