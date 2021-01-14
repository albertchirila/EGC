NUME: CHIRILA ALBERT
GRUPA: 334CB

	CREARE JUCATOR

		Jucatorul este format din doua mesh-uri, arc si sageata. Arcul este format din linie verticala
si arc de cerc, iar sageata dintr-un dreptunghi(2 triunghiuri) si un triunghi(varful sagetii).
		Jucatorul(arcul + sageata) se misca cu ajutorul tastelor W/S pe axa Oy, iar lansarea sagetii se face
cu ajutorul functiei "MoveArrow" pe axa Ox. Ambele obiecte se rotesc cu valoarea "angle" calculata in functie de 
pozitia curenta a mouse-ului si pozitia curenta a arcului.
		
	CREARE SHURIKEN
		
		Shurikenul este format din 4 triunghiuri.
		Un shuriken este reprezentat printr-o structura de date ce contine factorii de translatie, scalare, 
unghiul de rotatie, matricea obiectului, si o variabila care anunta daca obiectul este in miscare. Jocul contine 
3 astfel de obicte, fiind pastrate intr-un vector de structuri. Fiecare shuriken are o pozitie initiala prestabilita
in functia Init().
		Shuriken-urile se deplaseaza pe axa Ox de la dreapta spre stanga.

	CREARE BALOANE
		
		Balonul este format din 2 mesh-uri: o elipsa si o polilinie.
		Asemenea unui shuriken, balonul este reprezentat si el print-o structura cu datele corespunzatoare.
Jocul contine 7 baloane si sunt pastrate intr-un vector de structuri, fiecare balon avand o pozitie initiala 
prestabilita.

	Miscarea Shuriken-ului se face cu ajutorul functiei "MoveShuriken". La iesirea din scena, obiectul
revine pe pozitia initiala. Analog se realizeaza si miscarea balonului. In momentul unei coliziuni, se distruge
obiectul, si se actualizeaza scorul. Distrugerea obiectului se face cu factor se scalare subunitar pana cand
acesta dispare. La coliziunea unui shuriken cu arcul, jucatorul pierde o viata.

	In momentul in care sageata paraseste scena, aceasta revine in pozitia curenta a arcului, deoarece arcul
se poate deplasa in momentul miscarii sagetii.

	PUTEREA DE LANSARE A SAGETII
		
		PowerBar-ul este un mesh reprezentat ca dreptunghi. Cat timp se tine apasat butonul stang al 
mouse-ului, viteza sagetii creste, iar dreptunghiul este scalat pe axa Ox pana la o limita setata. In momentul 
eliberarii butonului, sageata este lansata.

	AFISAREA PUNCTAJULUI
		
		Afisarea punctajului se face in consola.
		Dupa fiecare coliziune, se afiseaza obiectul lovit si noul scor. Cand jucatorul ramane fara vieti,
jocul se inchide, iar in consola se afiseaza scorul final.
