#include <db/Join.h>

using namespace db;

Join::Join(JoinPredicate *p, DbIterator *child1, DbIterator *child2) : p(p), child1(child1), child2(child2) {
    // TODO pa3.1: some code goes here
}

JoinPredicate *Join::getJoinPredicate() {
    // TODO pa3.1: some code goes here
    return p; 
}

std::string Join::getJoinField1Name() {
    // TODO pa3.1: some code goes here
    return child1->getTupleDesc().getFieldName(p->getField1());
}

std::string Join::getJoinField2Name() {
    // TODO pa3.1: some code goes here
    return child2->getTupleDesc().getFieldName(p->getField2());
}

const TupleDesc &Join::getTupleDesc() const {
    // TODO pa3.1: some code goes here
    return TupleDesc::merge(child1->getTupleDesc(), child2->getTupleDesc());
}

void Join::open() {
    // TODO pa3.1: some code goes here
    child1->open();
    child2->open();
}

void Join::close() {
    // TODO pa3.1: some code goes here
    child1->close(); 
    child2->close();
}

void Join::rewind() {
    // TODO pa3.1: some code goes here
    child1->rewind(); 
    child2->rewind();
}

std::vector<DbIterator *> Join::getChildren() {
    // TODO pa3.1: some code goes here
    return {child1, child2};
}

void Join::setChildren(std::vector<DbIterator *> children) {
    // TODO pa3.1: some code goes here
    child1 = children[0]; 
    child2 = children[1]; 
}

std::optional<Tuple> Join::fetchNext() {
    // TODO pa3.1: some code goes here
    while (child1->hasNext()) {
        Tuple t1 = child1->next();
        while (child2->hasNext()) {
            Tuple t2 = child2->next();
            if (p->filter(&t1, &t2)) {
               Tuple newTuple = Tuple(this->getTupleDesc());  //Still need to include recordID
               for(auto i = 0; i < t1.getTupleDesc().numFields(); i++){
                    newTuple.setField(i, &t1.getField(i)); 
               }
               for(auto i = 0; i < t2.getTupleDesc().numFields(); i++){
                    newTuple.setField(t1.getTupleDesc().numFields() + i, &t2.getField(i)); 
               }
               return newTuple; 
            }
        }
        child2->rewind();
    }
    return std::nullopt;
}
