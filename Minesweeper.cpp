#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <chrono>

bool czyBylyWylosowane(int iLiczba,int iLiczba2, int tab[],int tab2[], int ile)
{
	if (ile <= 0)
		return false;

	int i = 0;
	do
	{
		if (tab[i] == iLiczba) {
			if (tab2[i] == iLiczba2) {
				return true;
			}
		}
		i++;

	} while (i < ile);

	return false;
}

struct czysty {
	int x;
	int y;
	int counter;
};

class Gra {
private:
	int rozmiar;
	int liczbabomb;
	int** mapa_rozlozenie;
	int** mapa_stan;
	float diff;
	int stan_gry;
	sf::RectangleShape** mapa_rys;
	sf::RenderWindow* okno;
	sf::Clock mysz_delay;
	sf::Text** cyferki;
	sf::Font* czcionka;
	sf::Text wygrana;
	sf::Text restartuj;
	sf::Text przegrana;
public:
	Gra(int rozm,int bomby);
	bool tru_start(int rand1,int rand2,int posx, int posy);
	void generuj(int posx,int posy);
	void rysuj();
	void petla_gra();
	void licznik();
	void customizuj();
	void mysz_zaznacz(sf::Event& event);
	void odkryj(int krat_x,int krat_y);
	void zaznacz(int krat_x,int krat_y);
	void gra_dzialanie(sf::Event& event);
	void win_check();
	void reset();
};

Gra::Gra(int rozm,int bomby) {
	rozmiar = rozm;
	stan_gry = 3;
	liczbabomb = bomby;
	diff = 800 / rozmiar;

	czcionka = new sf::Font;
	czcionka->loadFromFile("Arialn.ttf");

	mapa_rys = new sf::RectangleShape * [rozmiar];
	cyferki = new sf::Text * [rozmiar];
	for (int i = 0; i < rozmiar; i++) {
		mapa_rys[i] = new sf::RectangleShape[rozmiar];
		cyferki[i] = new sf::Text[rozmiar];
	}

	mapa_stan = new int* [rozmiar];
	mapa_rozlozenie = new int* [rozmiar];
	for (int i = 0; i < rozmiar; i++) {

		mapa_stan[i] = new int[rozmiar];
		mapa_rozlozenie[i] = new int[rozmiar];
	}

	for (int i = 0; i < rozmiar; i++) {
		for (int j = 0; j < rozmiar; j++) {
			mapa_stan[i][j] = 0;
			mapa_rozlozenie[i][j] = 0;
			cyferki[i][j].setFont(*czcionka);
			cyferki[i][j].setCharacterSize(diff - 5);
			cyferki[i][j].setPosition(sf::Vector2f((diff/3.33) + i * diff, j * diff));
			
		}
	}

	wygrana.setFont(*czcionka);
	wygrana.setCharacterSize(40);
	wygrana.setFillColor(sf::Color::White);
	wygrana.setOutlineColor(sf::Color::Red);
	wygrana.setOutlineThickness(2);
	wygrana.setPosition(sf::Vector2f(50, 50));
	wygrana.setString("GRATULACJE WYGRALES");

	przegrana.setFont(*czcionka);
	przegrana.setCharacterSize(40);
	przegrana.setFillColor(sf::Color::White);
	przegrana.setOutlineColor(sf::Color::Red);
	przegrana.setOutlineThickness(2);
	przegrana.setPosition(sf::Vector2f(50, 50));
	przegrana.setString("PRZEGRALES BOMBA WYBUCHLA");

	restartuj.setFont(*czcionka);
	restartuj.setCharacterSize(30);
	restartuj.setFillColor(sf::Color::White);
	restartuj.setOutlineColor(sf::Color::Red);
	restartuj.setOutlineThickness(2);
	restartuj.setPosition(sf::Vector2f(50, 500));
	restartuj.setString("KLIKNIJ PRAWY CTRL ABY ZRESTARTOWAC");

	okno = new sf::RenderWindow(sf::VideoMode(800, 800), "SAPER");
	reset();
}

void Gra::generuj(int posx, int posy) {
	std::mt19937 mt{ static_cast<std::mt19937::result_type>(
		std::chrono::steady_clock::now().time_since_epoch().count()
		) };

	std::uniform_int_distribution<> randrozmiar{ 0,rozmiar - 1 };

	int counter = 0;
	int* losy1 = new int[liczbabomb];
	int* losy2 = new int[liczbabomb];

	while (counter != liczbabomb) {
		int liczba1 = randrozmiar(mt);
		int liczba2 = randrozmiar(mt);
		if (czyBylyWylosowane(liczba1, liczba2, losy1, losy2, counter) == false) {
			losy1[counter] = liczba1;
			losy2[counter] = liczba2;
			counter++;
		}
	}

	for (int i = 0; i < rozmiar; i++) {
		for (int j = 0; j < rozmiar; j++) {
			mapa_rozlozenie[i][j] = 0;
			mapa_stan[i][j] = 0;
		}
	}


	for (int i = 0; i < liczbabomb; i++) {

		mapa_rozlozenie[losy1[i]][losy2[i]] = -1;

	}

	
	licznik();
	customizuj();

	delete[] losy1;
	delete[] losy2;
}

void Gra::customizuj() {
	for (int i = 0; i < rozmiar; i++) {
		for (int j = 0; j < rozmiar; j++) {

			switch (mapa_rozlozenie[i][j]) {
			case -1:
				mapa_rys[i][j].setFillColor(sf::Color::Red);
				cyferki[i][j].setString(std::to_string(-1));
				break;
			case 0:
				mapa_rys[i][j].setFillColor(sf::Color(169, 169, 169));
				cyferki[i][j].setString(std::to_string(0));
				break;
			case 1:
				mapa_rys[i][j].setFillColor(sf::Color(0, 30, 0));
				cyferki[i][j].setString(std::to_string(1));
				cyferki[i][j].setFillColor(sf::Color(47, 255, 0));
				cyferki[i][j].setOutlineColor(sf::Color::Black);
				cyferki[i][j].setOutlineThickness(1);
				break;
			case 2:
				mapa_rys[i][j].setFillColor(sf::Color(0, 60, 0));
				cyferki[i][j].setString(std::to_string(2));
				cyferki[i][j].setFillColor(sf::Color(219, 94, 48));
				cyferki[i][j].setOutlineColor(sf::Color::Black);
				cyferki[i][j].setOutlineThickness(1);
				break;
			case 3:
				mapa_rys[i][j].setFillColor(sf::Color(0, 90, 0));
				cyferki[i][j].setString(std::to_string(3));
				cyferki[i][j].setFillColor(sf::Color(245, 241, 32));
				cyferki[i][j].setOutlineColor(sf::Color::Black);
				cyferki[i][j].setOutlineThickness(1);
				break;
			case 4:
				mapa_rys[i][j].setFillColor(sf::Color(0, 120, 0));
				cyferki[i][j].setString(std::to_string(4));
				cyferki[i][j].setFillColor(sf::Color(74, 146, 250));
				cyferki[i][j].setOutlineColor(sf::Color::Black);
				cyferki[i][j].setOutlineThickness(1);
				break;
			case 5:
				mapa_rys[i][j].setFillColor(sf::Color(0, 150, 0));
				cyferki[i][j].setString(std::to_string(5));
				cyferki[i][j].setFillColor(sf::Color(255, 105, 252));
				cyferki[i][j].setOutlineColor(sf::Color::Black);
				cyferki[i][j].setOutlineThickness(1);
				break;
			case 6:
				mapa_rys[i][j].setFillColor(sf::Color(0, 180, 0));
				cyferki[i][j].setString(std::to_string(6));
				cyferki[i][j].setFillColor(sf::Color(186, 7, 49));
				cyferki[i][j].setOutlineColor(sf::Color::Black);
				cyferki[i][j].setOutlineThickness(1);
				break;
			case 7:
				mapa_rys[i][j].setFillColor(sf::Color(0, 210, 0));
				cyferki[i][j].setString(std::to_string(7));
				cyferki[i][j].setFillColor(sf::Color(255, 201, 214));
				cyferki[i][j].setOutlineColor(sf::Color::Black);
				cyferki[i][j].setOutlineThickness(1);
				break;
			case 8:
				mapa_rys[i][j].setFillColor(sf::Color(0, 240, 0));
				cyferki[i][j].setString(std::to_string(8));
				cyferki[i][j].setFillColor(sf::Color(143, 40, 14));
				cyferki[i][j].setOutlineColor(sf::Color::Black);
				cyferki[i][j].setOutlineThickness(1);
				break;
			}



			mapa_rys[i][j].setOutlineColor(sf::Color::White);
			mapa_rys[i][j].setOutlineThickness(1);
			mapa_rys[i][j].setPosition(sf::Vector2f(0 + i * diff, 0 + j * diff));
			mapa_rys[i][j].setSize(sf::Vector2f(diff, diff));
		}
	}
}

bool Gra::tru_start(int rand1, int rand2, int posx, int posy) {
	if (rand1 != posx && rand2 != posy) {
		//skrajne i tak dalej krok po kroku nie mam sily


	}
	else {
		return false;
	}
}

void Gra::rysuj() {

	for (int i = 0; i < rozmiar; i++) {
		for (int j = 0; j < rozmiar; j++) {
			okno->draw(mapa_rys[i][j]);
			if (mapa_stan[i][j] == 1 && (mapa_rozlozenie[i][j] != 0 && mapa_rozlozenie[i][j] != -1)) {
				okno->draw(cyferki[i][j]);
			}
		}
	}
}

void Gra::odkryj(int krat_x, int krat_y) {

	if (mapa_stan[krat_x][krat_y] == 0) {
		if (mapa_rozlozenie[krat_x][krat_y] == 0) {

			std::vector<czysty> stos;
			czysty first = { krat_x,krat_y,0 };
			stos.push_back(first);
			mapa_stan[krat_x][krat_y] = 1;

			while (stos.size() != 0) {
				
				
				
				int x_i = stos[stos.size() - 1].x;
				int y_i = stos[stos.size() - 1].y;

				switch (stos[stos.size() - 1].counter) {
					
				case 0:
					stos[stos.size() - 1].counter++;
					if (x_i != rozmiar - 1 && y_i != rozmiar - 1) {
						switch (mapa_rozlozenie[x_i + 1][y_i + 1]) {
						case 0:
							if (mapa_stan[x_i + 1][y_i + 1] == 0) {
								
								czysty next = { x_i + 1,y_i + 1,0 };
								stos.push_back(next);
								mapa_stan[x_i + 1][y_i + 1] = 1;
							}
							break;
						case 1:case 2: case 3: case 4: case 5: case 6: case 7: case 8:
							if (mapa_stan[x_i + 1][y_i + 1] == 0) {
								mapa_stan[x_i + 1][y_i + 1] = 1;
								
							}
							break;
						}
					}
					
					
					break;

				case 1:
					stos[stos.size() - 1].counter++;
					if (y_i != rozmiar - 1) {
						switch (mapa_rozlozenie[x_i][y_i + 1]) {
						case 0:
							if (mapa_stan[x_i][y_i + 1] == 0) {
								
								czysty next = { x_i,y_i + 1,0 };
								stos.push_back(next);
								mapa_stan[x_i][y_i + 1] = 1;
							}
							break;
						case 1:case 2: case 3: case 4: case 5: case 6: case 7: case 8:
							if (mapa_stan[x_i][y_i + 1] == 0) {
								mapa_stan[x_i][y_i + 1] = 1;
								
							}
							break;
						}
					}
					
					break;

				case 2:
					stos[stos.size() - 1].counter++;
					if (x_i != 0 && y_i != rozmiar - 1) {
						switch (mapa_rozlozenie[x_i - 1][y_i + 1]) {
						case 0:
							if (mapa_stan[x_i - 1][y_i + 1] == 0) {
								
								czysty next = { x_i - 1,y_i + 1,0 };
								stos.push_back(next);
								mapa_stan[x_i - 1][y_i + 1] = 1;
								
							}
							break;
						case 1:case 2: case 3: case 4: case 5: case 6: case 7: case 8:
							if (mapa_stan[x_i - 1][y_i + 1] == 0) {
								mapa_stan[x_i - 1][y_i + 1] = 1;
								
								
							}
							break;
						}
					}
					
					break;

				case 3:
					stos[stos.size() - 1].counter++;
					if (x_i != 0) {
						switch (mapa_rozlozenie[x_i - 1][y_i]) {
						case 0:
							if (mapa_stan[x_i - 1][y_i] == 0) {
								
								czysty next = { x_i - 1,y_i,0 };
								stos.push_back(next);
								mapa_stan[x_i - 1][y_i] = 1;
							}
							break;
						case 1:case 2: case 3: case 4: case 5: case 6: case 7: case 8:
							if (mapa_stan[x_i - 1][y_i] == 0) {
								mapa_stan[x_i - 1][y_i] = 1;
								
							}
							break;

						}
					}
					
					break;

				case 4:
					stos[stos.size() - 1].counter++;
					if (x_i != 0 && y_i != 0) {
						switch (mapa_rozlozenie[x_i - 1][y_i - 1]) {
						case 0:
							if (mapa_stan[x_i - 1][y_i - 1] == 0) {
								
								czysty next = { x_i - 1,y_i - 1,0 };
								stos.push_back(next);
								mapa_stan[x_i - 1][y_i - 1] = 1;
							}
							break;
						case 1:case 2: case 3: case 4: case 5: case 6: case 7: case 8:
							if (mapa_stan[x_i - 1][y_i - 1] == 0) {
								mapa_stan[x_i - 1][y_i - 1] = 1;
								
							}
							break;

						}
					}
					
					break;

				case 5:
					stos[stos.size() - 1].counter++;
					if (y_i != 0) {
						switch (mapa_rozlozenie[x_i][y_i - 1]) {
						case 0:
							if (mapa_stan[x_i][y_i - 1] == 0) {
								
								czysty next = { x_i,y_i - 1,0 };
								stos.push_back(next);
								mapa_stan[x_i][y_i - 1] = 1;
							}
							break;
						case 1:case 2: case 3: case 4: case 5: case 6: case 7: case 8:
							if (mapa_stan[x_i][y_i - 1] == 0) {
								mapa_stan[x_i][y_i - 1] = 1;
								
							}
							break;

						}
					}
				
					break;
				case 6:
					stos[stos.size() - 1].counter++;
					if (y_i != 0 && x_i != rozmiar - 1) {
						switch (mapa_rozlozenie[x_i + 1][y_i - 1]) {
						case 0:
							if (mapa_stan[x_i + 1][y_i - 1] == 0) {
								
								czysty next = { x_i + 1,y_i - 1,0 };
								stos.push_back(next);
								mapa_stan[x_i + 1][y_i - 1] = 1;
							}
							break;
						case 1:case 2: case 3: case 4: case 5: case 6: case 7: case 8:
							if (mapa_stan[x_i + 1][y_i - 1] == 0) {
								mapa_stan[x_i + 1][y_i - 1] = 1;
								
							}
							break;

						}
					}
					
					break;
				case 7:
					stos[stos.size() - 1].counter++;
					if (x_i != rozmiar - 1) {
						switch (mapa_rozlozenie[x_i + 1][y_i]) {
						case 0:
							if (mapa_stan[x_i + 1][y_i] == 0) {
								
								czysty next = { x_i + 1,y_i ,0 };
								stos.push_back(next);
								mapa_stan[x_i + 1][y_i] = 1;
							}
							break;
						case 1:case 2: case 3: case 4: case 5: case 6: case 7: case 8:
							if (mapa_stan[x_i + 1][y_i] == 0) {
								mapa_stan[x_i + 1][y_i] = 1;
								
							}
							break;

						}
					}
					
					break;

				case 8:
					stos.pop_back();
					break;
				}

			}
		}
		else if (mapa_rozlozenie[krat_x][krat_y] == -1) {
		stan_gry = 2;
		mapa_stan[krat_x][krat_y] = 1;

		}
		else {
			mapa_stan[krat_x][krat_y] = 1;



		}
	






	}
	for (int i = 0; i < rozmiar; i++) {
		for (int j = 0; j < rozmiar; j++) {
			if (mapa_stan[i][j] == 1) {
				mapa_rys[i][j].setFillColor(sf::Color(100, 100, 100));
				mapa_rys[i][j].setOutlineColor(sf::Color(50, 50, 50));
				if (mapa_rozlozenie[i][j] == -1) {
					mapa_rys[i][j].setFillColor(sf::Color::Red);
					mapa_rys[i][j].setOutlineColor(sf::Color(50, 50, 50));
				}

			}
		}
	}


}

void Gra::win_check() {
	int rapapa = 0;
	int all_check = 0;
	

	for (int i = 0; i < rozmiar; i++) {
		for (int j = 0; j < rozmiar; j++) {
			if (mapa_rozlozenie[i][j] == -1 && mapa_stan[i][j] == 2) {
				rapapa++;
				
			}
		}
	}
	if (rapapa == liczbabomb) {
		for (int i = 0; i < rozmiar; i++) {
			for (int j = 0; j < rozmiar; j++) {
				if (mapa_stan[i][j] != 0) {
					all_check++;
					
				}
			}
		}
	}
	if (all_check == rozmiar*rozmiar) {
		stan_gry = 1;
		
	}

	

}

void Gra::zaznacz(int krat_x, int krat_y) {
	if (mapa_stan[krat_x][krat_y] == 0) {
		mapa_stan[krat_x][krat_y] = 2;

	}
	else if (mapa_stan[krat_x][krat_y] == 2) {
		mapa_stan[krat_x][krat_y] = 0;
	}

	for (int i = 0; i < rozmiar; i++) {
		for (int j = 0; j < rozmiar; j++) {
			if (mapa_stan[i][j] == 2) {
				mapa_rys[i][j].setFillColor(sf::Color::Black);
				mapa_rys[i][j].setOutlineColor(sf::Color(50, 50, 50));
			}
			else if (mapa_stan[i][j] == 0) {
				mapa_rys[i][j].setFillColor(sf::Color(169, 169, 169));
				mapa_rys[i][j].setOutlineColor(sf::Color::White);
			}
		}
	}

}

void Gra::reset() {
	for (int i = 0; i < rozmiar; i++) {
		for (int j = 0; j < rozmiar; j++) {
			mapa_rozlozenie[i][j] = 0;
			mapa_stan[i][j] = 0;
		}
	}
	customizuj();
}

void Gra::mysz_zaznacz(sf::Event& event) {

	if (stan_gry == 0) {
		if (mysz_delay.getElapsedTime().asSeconds() > 0.08) {
			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.key.code == sf::Mouse::Left) {
					float pos_x = (sf::Mouse::getPosition(*okno).x) / diff;
					float pos_y = (sf::Mouse::getPosition(*okno).y) / diff;
					odkryj(int(pos_x), int(pos_y));

				}
				else if (event.key.code == sf::Mouse::Right) {
					float pos_x = (sf::Mouse::getPosition(*okno).x) / diff;
					float pos_y = (sf::Mouse::getPosition(*okno).y) / diff;
					zaznacz(int(pos_x), int(pos_y));

				}
				mysz_delay.restart();
			}
		}
	}
	else if (stan_gry == 3) {
		if (event.type == sf::Event::MouseButtonPressed) {
			if (event.key.code == sf::Mouse::Left) {
				float pos_x = (sf::Mouse::getPosition(*okno).x) / diff;
				float pos_y = (sf::Mouse::getPosition(*okno).y) / diff;
				generuj(int(pos_x), int(pos_y));
				odkryj(int(pos_x), int(pos_y));
				stan_gry = 0;
				mysz_delay.restart();
			}
		}
	}
}

void Gra::gra_dzialanie(sf::Event& event) {
	switch (stan_gry) {
	case 0:
		//w trakcie gry
		mysz_zaznacz(event);
		rysuj();
		win_check();
		break;

	case 1:
		//win
		rysuj();
		okno->draw(wygrana);
		okno->draw(restartuj);
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::RControl) {
				stan_gry = 3;
				reset();
			}
		}

		break;

	case 2:
		//lose
		rysuj();
		okno->draw(przegrana);
		okno->draw(restartuj);
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::RControl) {
				stan_gry = 3;
				reset();
			}
		}
		break;

	case 3:
		//start
		rysuj();
		mysz_zaznacz(event);
		break;

	}
}

void Gra::petla_gra() {

	while (okno->isOpen()) {
		
		sf::Event event;
		while (okno->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				okno->close();

			}
		}
		
		okno->clear();
		gra_dzialanie(event);


		okno->display();

	}
}

void Gra::licznik() {

	//skrajne
	for (int i = 0; i < rozmiar; i++) {
		if (mapa_rozlozenie[i][0] != -1) {
			if (i == 0) {
				if (mapa_rozlozenie[1][0] == -1) {
					mapa_rozlozenie[i][0]++;
				}
				if (mapa_rozlozenie[1][1] == -1) {
					mapa_rozlozenie[i][0]++;
				}
				if (mapa_rozlozenie[0][1] == -1) {
					mapa_rozlozenie[i][0]++;
				}
			}
			if (i == rozmiar - 1) {
				if (mapa_rozlozenie[rozmiar - 2][0] == -1) {
					mapa_rozlozenie[rozmiar - 1][0]++;
				}
				if (mapa_rozlozenie[rozmiar - 2][1] == -1) {
					mapa_rozlozenie[rozmiar - 1][0]++;
				}
				if (mapa_rozlozenie[rozmiar - 1][1] == -1) {
					mapa_rozlozenie[rozmiar - 1][0]++;
				}
			}
			if (i != 0 && i != rozmiar - 1) {
				if (mapa_rozlozenie[i - 1][1] == -1) {
					mapa_rozlozenie[i][0]++;
				}
				if (mapa_rozlozenie[i][1] == -1) {
					mapa_rozlozenie[i][0]++;
				}
				if (mapa_rozlozenie[i + 1][1] == -1) {
					mapa_rozlozenie[i][0]++;
				}
				if (mapa_rozlozenie[i - 1][0] == -1) {
					mapa_rozlozenie[i][0]++;
				}
				if (mapa_rozlozenie[i + 1][0] == -1) {
					mapa_rozlozenie[i][0]++;
				}
			}

		}
		if (mapa_rozlozenie[i][rozmiar - 1] != -1) {
			if (i == 0) {
				if (mapa_rozlozenie[i][rozmiar - 2] == -1) {
					mapa_rozlozenie[i][rozmiar - 1]++;
				}
				if (mapa_rozlozenie[i + 1][rozmiar - 1] == -1) {
					mapa_rozlozenie[i][rozmiar - 1]++;
				}
				if (mapa_rozlozenie[i + 1][rozmiar - 2] == -1) {
					mapa_rozlozenie[i][rozmiar - 1]++;
				}
			}
			if (i == rozmiar - 1) {
				if (mapa_rozlozenie[rozmiar - 1][rozmiar - 2] == -1) {
					mapa_rozlozenie[rozmiar - 1][rozmiar - 1]++;
				}
				if (mapa_rozlozenie[rozmiar - 2][rozmiar - 1] == -1) {
					mapa_rozlozenie[rozmiar - 1][rozmiar - 1]++;
				}
				if (mapa_rozlozenie[rozmiar - 2][rozmiar - 2] == -1) {
					mapa_rozlozenie[rozmiar - 1][rozmiar - 1]++;
				}
			}
			if (i != 0 && i != rozmiar - 1) {
				if (mapa_rozlozenie[i - 1][rozmiar - 1] == -1) {
					mapa_rozlozenie[i][rozmiar - 1]++;
				}
				if (mapa_rozlozenie[i + 1][rozmiar - 1] == -1) {
					mapa_rozlozenie[i][rozmiar - 1]++;
				}
				if (mapa_rozlozenie[i + 1][rozmiar - 2] == -1) {
					mapa_rozlozenie[i][rozmiar - 1]++;
				}
				if (mapa_rozlozenie[i][rozmiar - 2] == -1) {
					mapa_rozlozenie[i][rozmiar - 1]++;
				}
				if (mapa_rozlozenie[i - 1][rozmiar - 2] == -1) {
					mapa_rozlozenie[i][rozmiar - 1]++;
				}
			}
		}
		if (i != 0 && i != rozmiar - 1) {
			if (mapa_rozlozenie[0][i] != -1) {
				if (mapa_rozlozenie[0][i - 1] == -1) {
					mapa_rozlozenie[0][i]++;
				}
				if (mapa_rozlozenie[0][i + 1] == -1) {
					mapa_rozlozenie[0][i]++;
				}
				if (mapa_rozlozenie[1][i - 1] == -1) {
					mapa_rozlozenie[0][i]++;
				}
				if (mapa_rozlozenie[1][i] == -1) {
					mapa_rozlozenie[0][i]++;
				}
				if (mapa_rozlozenie[1][i + 1] == -1) {
					mapa_rozlozenie[0][i]++;
				}
			}
			if (mapa_rozlozenie[rozmiar - 1][i] != -1) {
				if (mapa_rozlozenie[rozmiar - 1][i - 1] == -1) {
					mapa_rozlozenie[rozmiar - 1][i]++;
				}
				if (mapa_rozlozenie[rozmiar - 1][i + 1] == -1) {
					mapa_rozlozenie[rozmiar - 1][i]++;
				}
				if (mapa_rozlozenie[rozmiar - 2][i - 1] == -1) {
					mapa_rozlozenie[rozmiar - 1][i]++;
				}
				if (mapa_rozlozenie[rozmiar - 2][i] == -1) {
					mapa_rozlozenie[rozmiar - 1][i]++;
				}
				if (mapa_rozlozenie[rozmiar - 2][i + 1] == -1) {
					mapa_rozlozenie[rozmiar - 1][i]++;
				}
			}

		}
	}

	for (int i = 1; i < rozmiar - 1; i++) {
		for (int j = 1; j < rozmiar - 1; j++) {
			if (mapa_rozlozenie[i][j] != -1) {
				if (mapa_rozlozenie[i - 1][j] == -1) {
					mapa_rozlozenie[i][j]++;
				}
				if (mapa_rozlozenie[i - 1][j + 1] == -1) {
					mapa_rozlozenie[i][j]++;
				}
				if (mapa_rozlozenie[i - 1][j - 1] == -1) {
					mapa_rozlozenie[i][j]++;
				}
				if (mapa_rozlozenie[i][j + 1] == -1) {
					mapa_rozlozenie[i][j]++;
				}
				if (mapa_rozlozenie[i][j - 1] == -1) {
					mapa_rozlozenie[i][j]++;
				}
				if (mapa_rozlozenie[i + 1][j] == -1) {
					mapa_rozlozenie[i][j]++;
				}
				if (mapa_rozlozenie[i + 1][j + 1] == -1) {
					mapa_rozlozenie[i][j]++;
				}
				if (mapa_rozlozenie[i + 1][j - 1] == -1) {
					mapa_rozlozenie[i][j]++;
				}
			}
		}
	}


	


}

int main() {
	srand(time(NULL));

	Gra gra(15, 33);
	gra.petla_gra();

}