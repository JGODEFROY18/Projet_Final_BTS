#include "CalculJson.h"

float CalculJson::QByteArrayToFloat(QByteArray arr)
{
	static_assert(std::numeric_limits<float>::is_iec559, "Only supports IEC 559 (IEEE 754) float");

	quint32 temp = ((quint8)arr[0] << 24) | ((quint8)arr[1] << 16) | ((quint8)arr[2] << 8) | (quint8)arr[3];
	//La variable temp est un entier de 32 bits, arr est un tableau de 4 cases d'entiers de 8 bits.
	//En partant de la fin, le programme place arr[0] au bit 24 puis arr[0] au bit 16 puis arr[0] au bit 8 et arr[0] au bit 0

	float* out = reinterpret_cast<float*>(&temp);
	//le compilateur va forcer la transformation d'un quint32 en float

	return *out;
}

QJsonValue CalculJson::valeurJson(QByteArray arr, char car)
{
	float val = QByteArrayToFloat(arr);
	QString affichage;
	switch (car) {
	case 'T':
		affichage = QString::number(double(val), 'f', 1) + 0xB0 + "C"; // 0xB0 = '°'
		//affiche la valeur avec une décimale
		break;
	case 'H':
		affichage = QString::number(double(val), 'f', 1) + "%";
		break;
	default:
		affichage = QString::number(double(val), 'f', 1);
		return 0;
	}
	QJsonValue valeur(affichage);
	return valeur;
}

float CalculJson::calculHumidite(int Vout) {
	float humid = -1.91 * pow(10, -9) * pow(Vout, 3);
	humid += 1.33 * pow(10, -5) * pow(Vout, 2);
	humid += 9.56 * pow(10, -3) * Vout;
	humid -= 2.16 * pow(10, 1);
	return humid;
}
