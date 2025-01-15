#include "object.h"
#include <set>
#include <iostream>
#include "variables.h"

std::unordered_set<std::shared_ptr<Object>> all_objects;
int l;
// std::vector<std::pair<Object*, bool>> teg;
Object::~Object() {
    l--;
};

void Object::ResetAll() {
    dependant_.clear();
}

Object::Object() {
    all_objects.insert(std::shared_ptr<Object>(this));
    l++;
}

std::shared_ptr<Object> Object::GetRef() {
    return shared_from_this();
}

void Object::AddDependant(std::shared_ptr<Object> son) {
    if (son) {
        dependant_.insert(son);
    }
}

void Object::ResetMark() {
    marked_ = false;
}

void Object::DeleteDependant(std::shared_ptr<Object> son) {
    if (dependant_.contains(son)) {
        dependant_.erase(son);
    }
}

void Object::Mark() {
    marked_ = true;
    for (auto& s : dependant_) {
        if (!s->IsMarked()) {
            s->Mark();
        }
        if (Is<Symbol>(s) && Contains(s)) {
            auto f = Get(s);
            if (!f->IsMarked()) {
                f->Mark();
            }
        }
        if (Is<Symbol>(s) && LambdaContains(s)) {
            auto f = LambdaFind(s);
            if (!f->IsMarked()) {
                f->Mark();
            }
        }
    }
}

bool Object::IsMarked() {
    return marked_;
}
