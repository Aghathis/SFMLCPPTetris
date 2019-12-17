#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>
#include "TetraminoDef.hpp"
#define TILESIZE 32
#define SPSIZE 50

class Grid : public sf::Drawable, public sf::Transformable{
public:
	Grid(sf::Texture *texture){
		m_vertices.setPrimitiveType(sf::PrimitiveType::Quads);
		m_vertices.resize(10*24*4);
		update();
		m_texture = texture;
	}

	char* getTable(){
		return table;
	}

	void update(){
		sf::Color c;
		int wichTiles = 0;
		for(int i = 0; i < 10;i++){
			for(int j = 0; j < 24;j++){
				if(j<4) c = sf::Color(0,0,0);
				else if(table[i+j*10] != '\0'){
					char tiles = table[i+j*10];
					c = sf::Color(255,255,255);
					if(tiles == 'I')	wichTiles = 0;
					if(tiles == 'O')	wichTiles = 1;
					if(tiles == 'T')	wichTiles = 2;
					if(tiles == 'L')	wichTiles = 3;
					if(tiles == 'J')	wichTiles = 4;
					if(tiles == 'S')	wichTiles = 5;
					if(tiles == 'Z')	wichTiles = 6;
				}else{
					wichTiles = 9;
					c = sf::Color(64,64,64);
				}
				sf::Vertex *quads = &m_vertices[(i+j*10)*4];
				int tx = (wichTiles%5)*SPSIZE;
				int ty = (wichTiles/5)*SPSIZE;

				quads[0] = sf::Vertex(sf::Vector2f(i*TILESIZE,j*TILESIZE),c,					sf::Vector2f(tx,ty));
				quads[1] = sf::Vertex(sf::Vector2f(i*TILESIZE+TILESIZE,j*TILESIZE),c,			sf::Vector2f(tx+SPSIZE,ty));
				quads[2] = sf::Vertex(sf::Vector2f(i*TILESIZE+TILESIZE,j*TILESIZE+TILESIZE),c,	sf::Vector2f(tx+SPSIZE,ty+SPSIZE));
				quads[3] = sf::Vertex(sf::Vector2f(i*TILESIZE,j*TILESIZE+TILESIZE),c,			sf::Vector2f(tx,ty+SPSIZE));

			}
		}
	}

	int checkLine(){
		int howMany = 0;
		for(int j = 0; j < 24;j++){
			int i;
			for(i = 0; i < 10;i++){
				if(table[i+j*10] == '\0'){
					break;
				}
			}
			if(i==10){
				howMany++;
				for(i = 0; i < 10;i++){
					table[i+j*10] = '\0';
				}
				for(int y = j; y > 0 ;y--){
					for(i = 0; i < 10;i++){
						table[i+y*10] = table[i+(y-1)*10];
						table[i+(y-1)*10] = '\0';
					}

				}
			}
		}
		return howMany;
	}
private:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const{
		states.transform*=getTransform();
		states.texture = m_texture;
		target.draw(m_vertices,states);
	}
private:
	sf::Texture *m_texture;
	sf::VertexArray m_vertices;
	char table[10*24] = {'\0'};
};

class Tetramino{
public:
	Tetramino(TetraminoInfo*pieces){
		newPieces(pieces);
	}

	void newPieces(TetraminoInfo* pieces){
		m_pieces = pieces;
		rotation = 0;
		x=3;
		y=0;
	}

	void rotate(int dir){
		rotation+=dir;
		if(rotation > 3) rotation = 0;
		if(rotation < 0) rotation = 3;
	}

	TetraminoInfo* getInfo(){
		return m_pieces;
	}

	int getRotation(){
		return rotation;
	}

    void fall(){
        y++;
    }

    void move(int dir){
        x+=dir;
    }

    void insert(char table[10*24]){
		for(int i = 0; i < 4;i++){
			for(int j = 0; j < 4;j++){
				if(x+i>=0 && x+i <10 && y+j >=0 && y+j <24)
					if(tetraminoBlock(m_pieces->shape[rotation],i,j)){
                        table[(x+i)+(y+j)*10] = m_pieces->name;
					}
			}
		}
    }

	bool fallColid(char table[10*24]){
		for(int i = 0; i < 4;i++){
			for(int j = 0; j < 4;j++){
				if(x+i>=0 && x+i <10 && y+j >=0 && y+j <24)
					if(tetraminoBlock(m_pieces->shape[rotation],i,j)){
						if(y+1+j > 23) return true;
						if(table[(x+i)+(y+1+j)*10] != '\0') return true;
					}
			}
		}
		return false;
	}

	bool moveColid(char table[10*24],int direction){
		for(int i = 0; i < 4;i++){
			for(int j = 0; j < 4;j++){
				if(tetraminoBlock(m_pieces->shape[rotation],i,j)){
					int xTest = x+i+direction;
					if(xTest<0){
						return true;
					}else if(xTest>9){
						return true;
					}else if(table[(xTest)+(y+j)*10] != '\0'){
                        return true;
					}
				}
			}
		}
		return false;
	}

	sf::Vector2f getGridPos(){
		return sf::Vector2f(x,y);
	}

	sf::Vector2f getPos(){
		return sf::Vector2f(x*TILESIZE,y*TILESIZE);
	}

private:
	int x;
	int y;
	short rotation = 0;
	TetraminoInfo *m_pieces;
};

class DrawableTetramino : public sf::Drawable, public sf::Transformable{
public:
	DrawableTetramino(Tetramino t,sf::Texture *texture){
		m_vertices.setPrimitiveType(sf::PrimitiveType::Quads);
		m_vertices.resize(16);
		recalculate(t);
		m_texture = texture;
	}
	DrawableTetramino(TetraminoInfo *info,int rotation,sf::Texture *texture){
		m_vertices.setPrimitiveType(sf::PrimitiveType::Quads);
		m_vertices.resize(16);
		recalculate(info,rotation);
		m_texture = texture;
	}

	DrawableTetramino(sf::Texture *texture){
		m_vertices.setPrimitiveType(sf::PrimitiveType::Quads);
		m_vertices.resize(16);
		m_texture = texture;
	}
	void color(sf::Color c){
		for(int i = 0; i < 16;i++){
			m_vertices[i].color = c;
		}
	}

    void recalculate(Tetramino t){
		m_pieces = t.getInfo();
		rotation = t.getRotation();
		recalculateGeometry();
    }

	void recalculate(TetraminoInfo *info,int rotation){
		m_pieces = info;
		this->rotation = rotation;
		recalculateGeometry();
    }

private:
	void recalculateGeometry(){
        //sf::Color c = m_pieces->color;
		int tiles = m_pieces->tiles;
		int tx = (tiles%5)*SPSIZE;
		int ty = (tiles/5)*SPSIZE;

		unsigned int s = m_pieces->shape[rotation];
		int b=0;
		for(int i = 0; i < 4;i++){
			for(int j = 0; j < 4;j++){
				if(tetraminoBlock(s,i,j)){
					sf::Vertex *quads = &m_vertices[b*4];/*
                    quads[0] = sf::Vertex(sf::Vector2f(i*TILESIZE,j*TILESIZE),c);
                    quads[1] = sf::Vertex(sf::Vector2f(i*TILESIZE+TILESIZE,j*TILESIZE),c);
                    quads[2] = sf::Vertex(sf::Vector2f(i*TILESIZE+TILESIZE,j*TILESIZE+TILESIZE),c);
                    quads[3] = sf::Vertex(sf::Vector2f(i*TILESIZE,j*TILESIZE+TILESIZE),c);*/
					quads[0] = sf::Vertex(sf::Vector2f(i*TILESIZE,j*TILESIZE),						sf::Vector2f(tx,ty));
                    quads[1] = sf::Vertex(sf::Vector2f(i*TILESIZE+TILESIZE,j*TILESIZE),				sf::Vector2f(tx+SPSIZE,ty));
                    quads[2] = sf::Vertex(sf::Vector2f(i*TILESIZE+TILESIZE,j*TILESIZE+TILESIZE),	sf::Vector2f(tx+SPSIZE,ty+SPSIZE));
                    quads[3] = sf::Vertex(sf::Vector2f(i*TILESIZE,j*TILESIZE+TILESIZE),				sf::Vector2f(tx,ty+SPSIZE));
                    b++;
				}
			}
		}
	}

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const{
		states.transform*=getTransform();
		states.texture = m_texture;
		target.draw(m_vertices,states);
	}
	sf::VertexArray m_vertices;
	TetraminoInfo *m_pieces;
	sf::Texture *m_texture;
	int rotation;
};

class RandomTetramino{
public:
	RandomTetramino(){
		index = 0;
		genList(0);
		genList(1);
	}

	TetraminoInfo* getNext(){
		if(index == 14){
			index = 0;
            for(int i = 0; i < 7;i++){
				tArray[i] = tArray[7+i];
            }
            genList(1);
		}
		return &TetraminoDef[tArray[index++]];
	}

	int* getnextPieces(){
		return &tArray[index+1];
	}

private:
	void genList(int liste){
		liste*=7;
		for(int i = 0; i < 7 ; i++){
            tArray[i+liste] = i;
		}
		for(int i = 0; i < 28; i++){
			std::swap(tArray[liste+rand()%7],tArray[liste+rand()%7]);
		}
	}

private:
	int tArray[14];
	int index;
};

int main(){
	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode(1024,640),"AMO",sf::Style::Close);
	window.setFramerateLimit(240);
	//window.setPosition(sf::Vector2i(400,1200));

	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\calibri.ttf");
	sf::Text text;
	text.setFont(font);


	sf::Texture texture;
	texture.loadFromFile("boxes.png");
	Grid grid(&texture);
	RandomTetramino randGen;

	Tetramino t(randGen.getNext());
	Tetramino phantom = t;

	DrawableTetramino drawt(t,&texture);
	DrawableTetramino drawPhantom(phantom,&texture);
	DrawableTetramino drawSaved(&texture);

	int saved = -1;
	int triggerShift = 0;
	float speed = 0.40;
	float fallCounter = 0;
	float colidCounter = 0;
	float moveCounter = 0;
	sf::Clock clock;

	int nbLigneCounter = 0;
	int score = 0;
	int level = 1;

	while(window.isOpen()){
		float elapsed = clock.restart().asSeconds();
		sf::Event event;
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed) window.close();
			if(event.type == sf::Event::KeyPressed){
				if(event.key.code == sf::Keyboard::A){
					t.rotate(-1);
					if(t.moveColid(grid.getTable(),0)){
						t.rotate(+1);
						colidCounter = 0;
					}
					drawt.recalculate(t);
				}else if(event.key.code == sf::Keyboard::E){
					t.rotate(+1);
					if(t.moveColid(grid.getTable(),0)){
						colidCounter = 0;
						t.rotate(-1);
					}
					drawt.recalculate(t);
				}else if(event.key.code == sf::Keyboard::LShift){
					if(triggerShift == 0){
						if(saved == -1){
							saved = t.getInfo()->tiles;
							t.newPieces(randGen.getNext());
						}else{
							int temp = t.getInfo()->tiles;
							t.newPieces(&TetraminoDef[saved]);
							saved = temp;
						}
						drawt.recalculate(t);
						triggerShift = 1;
					}
				}
			}
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
			moveCounter+=elapsed;
			if(moveCounter > 0.075){
				if(!t.moveColid(grid.getTable(),+1)){
					t.move(+1);
					colidCounter = 0;
				}
				moveCounter = 0;
			}
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
			moveCounter+=elapsed;
			if(moveCounter > 0.075){
				if(!t.moveColid(grid.getTable(),-1)){
					t.move(-1);
					colidCounter = 0;
				}
				moveCounter = 0;
			}
		}


		fallCounter+=elapsed;

		if(t.fallColid(grid.getTable())){
			colidCounter+=elapsed;
			if(colidCounter > 0.5){
				t.insert(grid.getTable());
				int code = grid.checkLine();
				if(code == -1 ){
					printf("FIN defaite !");
					return 0;
				}
				score+=(code*code)*4;
				nbLigneCounter += code;
				level = 1+nbLigneCounter/25;
				speed = (float)(50-level)/100;
				if(speed < 0) speed = 0;

				grid.update();
				t.newPieces(randGen.getNext());
				drawt.recalculate(t);
				colidCounter = 0;
				triggerShift = 0;
			}
		}else if(fallCounter > speed-((speed-0.05)*sf::Keyboard::isKeyPressed(sf::Keyboard::S)*(speed>0.05))){
			t.fall();
			fallCounter =0;
			score+=1*sf::Keyboard::isKeyPressed(sf::Keyboard::S);
		}

		phantom = t;
		drawPhantom.recalculate(phantom);
		drawPhantom.color(sf::Color(255,255,255,64));
		while(!phantom.fallColid(grid.getTable()))
			phantom.fall();

		std::stringstream ss;
		ss<<"score : "<<score<<"\nnombre ligne : "<<nbLigneCounter<<"\nlevel : "<<level<<std::endl;
		text.setString(ss.str());
		text.setFillColor(sf::Color::White);

		//============================================================DRAW=============================

		window.clear();
		sf::Vector2f offset(512-TILESIZE*5,-TILESIZE*4);
		grid.setPosition(offset);
		window.draw(grid);
		drawt.setPosition(t.getPos()+offset);
		window.draw(drawt);

		text.setPosition(1024-256,512-64);
		window.draw(text);

		drawPhantom.setPosition(phantom.getPos()+offset);
		window.draw(drawPhantom);


		if(saved != -1){
			drawSaved.recalculate(&TetraminoDef[saved],0);
			drawSaved.setPosition(sf::Vector2f(0,16));
			window.draw(drawSaved);
		}


		window.display();
	}



    return 0;
}
