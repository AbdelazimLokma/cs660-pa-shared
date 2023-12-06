#include <db/IntHistogram.h>

using namespace db;

IntHistogram::IntHistogram(int buckets, int min, int max): buckets(buckets), min(min), max(max), bucketCount(buckets, 0), totalValues(0) {
    // TODO pa4.1: some code goes here
}

void IntHistogram::addValue(int v) {
    // TODO pa4.1: some code goes here
    int index = (v - min) * buckets / (max - min + 1);
    bucketCount[index]++;
    totalValues++;
}

double IntHistogram::estimateSelectivity(Predicate::Op op, int v) const {
    // TODO pa4.1: some code goes here
    int index = (v - min) * buckets / (max - min + 1);
    double width = static_cast<double>(max - min + 1) / buckets;
    double selectivity = 0.0;
    switch (op){
        case Predicate::Op::EQUALS:
            if (v < min || v > max) return 0.0;
            selectivity = static_cast<double>(bucketCount[index]) / width / totalValues;
            break;
        case Predicate::Op::NOT_EQUALS:
            if (v < min || v > max) return 1.0;
            selectivity = 1 - static_cast<double>(bucketCount[index]) / width / totalValues;
            break;
        case Predicate::Op::LESS_THAN:
        case Predicate::Op::LESS_THAN_OR_EQ:
            for (int i = 0; i < index; i++){
                selectivity += static_cast<double>(bucketCount[i] / totalValues);
            }
            if (op == Predicate::Op::LESS_THAN_OR_EQ){
                double bucketFraction = (v - index * width) / width;
                selectivity += bucketFraction * bucketCount[index] / totalValues;
            }
            break;
        case Predicate::Op::GREATER_THAN:
        case Predicate::Op::GREATER_THAN_OR_EQ:
            for(int i = index + 1; i < buckets; i++){
                selectivity += static_cast<double>(bucketCount[i] / totalValues);
            }
            if (op == Predicate::Op::GREATER_THAN_OR_EQ){
                double bucketFraction = (width - (v - index * width)) / width;
                selectivity += bucketFraction * bucketCount[index] / totalValues;
            }
            break;
            }

        return selectivity;
    }


double IntHistogram::avgSelectivity() const {
    // TODO pa4.1: some code goes here
}

std::string IntHistogram::to_string() const {
    // TODO pa4.1: some code goes here
}
