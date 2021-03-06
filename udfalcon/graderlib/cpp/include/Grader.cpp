#include <memory>
#include "json/json.h"
#include "Grader.h"
#include "RubricItem.h"

using namespace std;
typedef Json::Value json;

void Grader::run()
{
  for (auto i: items)
  {
    i->evaluate();
    stats.record(i);

    if (i->failed() && !i->optional)
      break;
  }
  has_evaluated = true;
}

void Grader::run_debug()
{
  for (auto i: items)
  {
    i->evaluate();
    stats.record(i);
  }
}

shared_ptr<RubricItem> Grader::createRubricItem()
{
  shared_ptr<RubricItem> item = make_shared<RubricItem>();
  postCreateRubricItem(item);
  return item;
}

shared_ptr<RubricItem> Grader::createRubricItem(const string& name)
{
  shared_ptr<RubricItem> item = make_shared<RubricItem>(name);
  postCreateRubricItem(item);
  return item;
}

shared_ptr<RubricItem> Grader::createRubricItem(const function<bool()>& _callback)
{
  shared_ptr<RubricItem> item = make_shared<RubricItem>(_callback);
  postCreateRubricItem(item);
  return item;
}

void Grader::postCreateRubricItem(shared_ptr<RubricItem> item)
{
  items.push_back(item);
  stats.num_created = static_cast<unsigned>(items.size());
}

bool Grader::passed()
{
  return stats.passed();
}

bool Grader::failed()
{
  return stats.failed();
}

json Grader::resultsJson()
{
  return stats.resultsJson();
}

string Grader::results()
{
  return stats.results();
}
