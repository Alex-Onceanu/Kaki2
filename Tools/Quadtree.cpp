#include "../Tools/pch.h"

#include "Quadtree.h"
#include "../Utility.h"

Direction PositionnementRelatifDiag(TextureRectMeta a, TextureRectMeta b)
{
	Direction tt[2][2] = { { Direction::NORD_OUEST, Direction::SUD_OUEST },{ Direction::NORD_EST,Direction::SUD_EST } };
	return tt[int(a.get_x() < b.get_x())][int(a.get_y() > b.get_y())];
}

Direction PositionnementRelatifDiag(TextureRectMeta a, Position& b)
{
	Direction tt[2][2] = { { Direction::NORD_OUEST, Direction::SUD_OUEST },{ Direction::NORD_EST,Direction::SUD_EST } };
	return tt[int(a.get_x() < b.x)][int(a.get_y() > b.y)];
}


//____________________________________________________________________________TextureRectMeta____________________________________________________

TextureRectMeta::TextureRectMeta(std::shared_ptr<Texture> __img, const Rect& __rect)
{
	img = std::move(__img);
	rect = __rect;
}

int TextureRectMeta::get_x()
{
	return rect.x;
}

int TextureRectMeta::get_y()
{
	return rect.y;
}

int TextureRectMeta::get_w()
{
	return rect.w;
}

int TextureRectMeta::get_h()
{
	return rect.h;
}

Rect TextureRectMeta::get_rect()
{
	return rect;
}

std::shared_ptr<Texture> TextureRectMeta::get_img()
{
	return std::move(img);
}

Texture* TextureRectMeta::get_imgptr()
{
	return img.get();
}

bool operator==(TextureRectMeta& one, TextureRectMeta& other)
{
	//Compare chaque attribut independemment
	return (one.get_x() == other.get_x()) and (one.get_y() == other.get_y()) and (one.get_w() == other.get_w()) and (one.get_h() == other.get_h()) and (one.get_imgptr() == other.get_imgptr());
}

bool operator==(TextureRectMeta& one, Position& other)
{
	//Ne compare pas les textures ! verifie uniquement si les positions sont identiques
	return (one.get_x() == other.x) and (one.get_y() == other.y);
}

//__________________________________________________________________________Node_____________________________________________________________

Quadtree_node::Quadtree_node(Quadtree_node* upl, Quadtree_node* upr, const TextureRectMeta& v, Quadtree_node* downl, Quadtree_node* downr)	
{
	val = v;
	children[int(Direction::NORD_OUEST)] = upl;
	children[int(Direction::NORD_EST)] = upr;
	children[int(Direction::SUD_OUEST)] = downl;
	children[int(Direction::SUD_EST)] = downr;
}

bool Quadtree_node::is_leaf()
{
	return (std::count(children.begin(), children.end(), nullptr) == 4);
}

Quadtree_node* create_root(TextureRectMeta& elem)
{
	return new Quadtree_node(nullptr, nullptr, elem, nullptr, nullptr);
}

void push(Quadtree_node* root, TextureRectMeta elem)
{
	//racine non nulle
	if (root == nullptr) return;
	auto comparaison = PositionnementRelatifDiag(root->val, elem);

	push(root->children[int(comparaison)], elem);

	if (root->children[int(comparaison)] == nullptr)
	{
		root->children[int(comparaison)] = new Quadtree_node(nullptr, nullptr, elem, nullptr, nullptr);
	}
}

void parcours_postfixe(Quadtree_node* root, std::function<void(Quadtree_node*)> lambda)
{
	if (root == nullptr) return;

	for (auto&& c : root->children)
	{
		parcours_postfixe(c, lambda);
	}
	lambda(root);
}

void parcours_infixe(Quadtree_node* root, std::function<void(Quadtree_node*)> lambda)
{
	if (root == nullptr) return;

	parcours_infixe(root->children[int(Direction::NORD_OUEST)], lambda);
	parcours_infixe(root->children[int(Direction::NORD_EST)], lambda);

	lambda(root);

	parcours_infixe(root->children[int(Direction::SUD_OUEST)], lambda);
	parcours_infixe(root->children[int(Direction::SUD_EST)], lambda);
}

void parcours_inverse(Quadtree_node* root, std::function<void(Quadtree_node*)> lambda)
{
	if (root == nullptr) return;

	parcours_inverse(root->children[int(Direction::SUD_EST)], lambda);
	parcours_inverse(root->children[int(Direction::SUD_OUEST)], lambda);

	lambda(root);

	parcours_inverse(root->children[int(Direction::NORD_EST)], lambda);
	parcours_inverse(root->children[int(Direction::NORD_OUEST)], lambda);
}

Quadtree_node* find(Quadtree_node* root, TextureRectMeta& elem)
{
	if (root == nullptr) return nullptr;
	if (root->val == elem) return root;

	auto comparaison = PositionnementRelatifDiag(root->val, elem);
	return find(root->children[int(comparaison)], elem);
}

Quadtree_node* find(Quadtree_node* root, Position& pos)
{
	if (root == nullptr) return nullptr;
	if (root->val == pos) return root;

	auto comparaison = PositionnementRelatifDiag(root->val, pos);
	return find(root->children[int(comparaison)], pos);
}

void remove_node(Quadtree_node* root, Quadtree_node* node)
{
	if (root == nullptr) return;

	//On cherche le noeud parmi les 4 sous-branches de la racine
	auto f = std::find(root->children.begin(), root->children.end(), node);

	if (f == root->children.end())
	{
		//Si ce n'est pas une des sous-branches, on continue de s'enfoncer dans l'arbre
		auto comparaison = PositionnementRelatifDiag(root->val, node->val);
		return remove_node(root->children[int(comparaison)], node);
	}

	//Si root pointe vers le noeud qu'on cherche on fait d'abord en sorte qu'il l'oublie avant, puis on delete le noeud
	root->children[f - root->children.begin()] = nullptr;
	delete *f;
}

//___________________________________________________________________________tree_______________________________________________________________

Quadtree::Quadtree(Quadtree_node* __root)
{
	root = __root;
}

Quadtree::Quadtree(TextureRectMeta& elem)
{
	root = create_root(elem);
}

Quadtree::~Quadtree()
{
	Parcours_Postfixe([](Quadtree_node* c) {if (c) delete c; });
}

void Quadtree::Push(TextureRectMeta& elem)
{
	if (root == nullptr) root = create_root(elem);
	else push(root, elem);
}

void Quadtree::Remove(Quadtree_node* node)
{
	//Pas le droit de supprimer la racine d'un arbre sans supprimer ses sous-branches
	//Si le noeud a supprimer n'est pas la racine, on peut la supprimer elle et ses sous-branches
	//Pour verifier si la racine n'a pas d'enfants on compte le nombre de "nullptr" que son array de sous-branches contient

	if ((node == root) and root->is_leaf()) root = nullptr;
	else remove_node(root, node);
}

Quadtree_node* Quadtree::Find(TextureRectMeta& elem)
{
	return find(root, elem);
}

Quadtree_node* Quadtree::Find(Position& pos)
{
	return find(root, pos);
}

void Quadtree::Parcours_Infixe(std::function<void(Quadtree_node*)> lambda)
{
	parcours_infixe(root, lambda);
}

void Quadtree::Parcours_Postfixe(std::function<void(Quadtree_node*)> lambda)
{
	parcours_postfixe(root, lambda);
}

void Quadtree::Parcours_Inverse(std::function<void(Quadtree_node*)> lambda)
{
	parcours_inverse(root, lambda);
}
