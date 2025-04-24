class QuadTree {
    struct Node {
        sf::FloatRect bounds;
        std::vector<RigidBody*> bodies;
        std::array<std::unique_ptr<Node>, 4> children;
        static const int MAX_BODIES = 4;
        
        bool isLeaf() const { return !children[0]; }
        
        void split() {
            float x = bounds.left;
            float y = bounds.top;
            float w = bounds.width / 2;
            float h = bounds.height / 2;
            
            children[0] = std::make_unique<Node>(sf::FloatRect(x, y, w, h));
            children[1] = std::make_unique<Node>(sf::FloatRect(x + w, y, w, h));
            children[2] = std::make_unique<Node>(sf::FloatRect(x, y + h, w, h));
            children[3] = std::make_unique<Node>(sf::FloatRect(x + w, y + h, w, h));
        }
    };

    std::unique_ptr<Node> root;

public:
    QuadTree() {
        root = std::make_unique<Node>(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
    }

    void insert(RigidBody* body) {
        insertToNode(root.get(), body);
    }

private:
    void insertToNode(Node* node, RigidBody* body) {
        if (!node->isLeaf() && node->bodies.size() >= Node::MAX_BODIES) {
            node->split();
            // Redistribute bodies to children
        }
        node->bodies.push_back(body);
    }
};