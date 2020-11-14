#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <chrono>
#include <regex>

#define ITERATION 10000

//
// Test Gen
//

#define MAX_LENGTH 5
#define MIN_LENGTH 2
#define MAX_DEPTH 7
#define MAX_ATTRIBUTES 3

using namespace std;

const int empty_rate = 5;
const int has_value_rate = 35;
const int has_tags_rate = 60;
// sum of rates should be equal to 100

mt19937 seed(chrono::steady_clock::now().time_since_epoch().count());

vector<string> words;
ofstream fc_out;

int random(int x)
{
    uniform_int_distribution<int> uid(1, x);
    return uid(seed);
}

int random_left_right(int left, int right)
{
    uniform_int_distribution<int> uid(left, right);
    return uid(seed);
}

string random_word()
{
    return words[random(words.size()) - 1];
}

void random_xml(int depth, string tab)
{
    if (depth > MAX_DEPTH)
    {
        return;
    }

//    Start new tag
    string tag_name = random_word();
    string text = "<" + tag_name;

    if (random(2) > 1)
    {
        for (int i = random(MAX_ATTRIBUTES); i > 0; --i)
        {
            text += " " + random_word() + "=\"" + random_word() + "\"";
        }
    }

    int x = random(empty_rate + has_value_rate + (depth == MAX_DEPTH ? 0 : has_tags_rate));
    fc_out << tab;
    if (x <= empty_rate)
    {
        if (random(2) > 1)
        {
            text += "></" + tag_name + ">";
        } else {
            text += "/>";
        }
        fc_out << text << '\n';
        return;
    }

    x += empty_rate;
    if (x <= empty_rate + has_value_rate)
    {
        text += ">" + random_word() + "</" + tag_name + ">";
        fc_out << text << '\n';
        return;
    }

    text += ">";
    fc_out << text << '\n';

    for (int i = random_left_right(MIN_LENGTH, MAX_LENGTH); i > 0; --i)
    {
        // Generate more node recursively
        random_xml(depth + 1, tab + "\t");
    }

//    End of tag
    fc_out << tab << "</" + tag_name + ">" << '\n';
}

//
// End of Test Gen
//


// #include "xml.h"
extern "C"
{
#include "xml.c"
}

class foo
{
public:
    void callfunc()
    {
        exe();
    }
};

int main() {

    std::cout << "Hello, World!" << std::endl;

    std::cout << "Loading words from dictionary..." << std::endl;

    ifstream fc_in("data/dictionary.txt");
    string word;

    while (fc_in >> word)
    {
        regex reg("[.,\\[\\]]");
        word = regex_replace(word, reg, "");
        if (!word.empty())
        {
            words.push_back(word);
        }
    }

    fc_in.close();

    int i = ITERATION;
    while (i--)
    {
        fc_out.open("data/file.xml");
        printf("iteration %d\n", ITERATION - i);

        // Generate a xml file to '~/data/file.xml'

        random_xml(1, "");

        fc_out.close();

        // Call exe() (xml converter) function from 'xml.h'

         foo func;

         func.callfunc();

        // End of each iteration
    }

    return 0;
}
