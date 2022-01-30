#include <array>
#include <memory>
#include <functional>
#include <memory>

#include "../Rect.h"
#include "../Texture.h"
#include "../Position.h"

enum class Direction
{
	NORD_OUEST,
	NORD_EST,
	SUD_OUEST,
	SUD_EST,
};

class TextureRectMeta
{
public:
	TextureRectMeta() { rect = { 0 }; img = nullptr; };
	TextureRectMeta(std::shared_ptr<Texture> __img, const Rect& __rect);
	int get_x();
	int get_y();
	int get_w();
	int get_h();
	Rect get_rect();
	std::shared_ptr<Texture> get_img();
	Texture* get_imgptr();

protected:
	std::shared_ptr<Texture> img;
	Rect rect;

	friend bool operator==(TextureRectMeta& one, TextureRectMeta& other);
	friend bool operator==(TextureRectMeta& one, Position& other);
};

class Quadtree_node
{
public:
	Quadtree_node(std::shared_ptr<TextureRectMeta> __val, Rect __zone);
	bool is_leaf();
	bool is_empty();
	bool is_empty_but_not_leaf();

	Rect zone;
	std::shared_ptr<TextureRectMeta> val;
	std::array<Quadtree_node*, 4> children = { nullptr,nullptr,nullptr,nullptr };

};

class Quadtree
{
public:
	Quadtree() { root = nullptr; };
	Quadtree(Rect ecran);
	~Quadtree();

	Quadtree_node* Push(std::shared_ptr<TextureRectMeta> elem);
	void Remove(Quadtree_node* node);
	Quadtree_node* Find(Position& pos);
	void Parcours_Postfixe(std::function<void(Quadtree_node*)> lambda);
	void Parcours_Infixe(std::function<void(Quadtree_node*)> lambda);
	void Parcours_Infixe_Collision(Rect ecran, std::function<void(Quadtree_node*)> lambda);

private:
	Quadtree_node* root;
};

#if 0

class TextureRectMeta
{
public:
	TextureRectMeta() {rect = { 0 }; img = nullptr;};
	TextureRectMeta(std::shared_ptr<Texture> __img, const Rect& __rect);
	int get_x();
	int get_y();
	int get_w();
	int get_h();
	Rect get_rect();
	std::shared_ptr<Texture> get_img();
	Texture* get_imgptr();

protected:
	std::shared_ptr<Texture> img;
	Rect rect;

	friend bool operator==(TextureRectMeta& one, TextureRectMeta& other);
	friend bool operator==(TextureRectMeta& one, Position& other);
};

class Quadtree_node
{
public:
	Quadtree_node(Quadtree_node* upl, Quadtree_node* upr, const TextureRectMeta& v, Quadtree_node* downl, Quadtree_node* downr);
	TextureRectMeta val;
	std::array<Quadtree_node*, 4> children;
	bool is_leaf();
};

class Quadtree
{
public:
	Quadtree(Quadtree_node* __root);
	Quadtree(TextureRectMeta& elem);
	~Quadtree();

	void Push(TextureRectMeta& elem);
	void Remove(Quadtree_node* node);
	Quadtree_node* Find(TextureRectMeta& elem);
	Quadtree_node* Find(Position& pos);
	void Parcours_Infixe(std::function<void(Quadtree_node*)> lambda);
	void Parcours_Postfixe(std::function<void(Quadtree_node*)> lambda);
	void Parcours_Inverse(std::function<void(Quadtree_node*)> lambda);

private:
	Quadtree_node* root;
};

enum class Direction
{
	NORD_OUEST,
	NORD_EST,
	SUD_OUEST,
	SUD_EST,
};

#endif