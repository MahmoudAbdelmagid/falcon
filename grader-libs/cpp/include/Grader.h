#ifndef Grader_H_
#define Grader_H_

#include <memory>
#include <string>
#include <vector>
#include "Stats.h"
#include "RubricItem.h"

class Grader
{
public:
  bool has_run = false;
  Stats stats;
  size_t num_tests;

  std::shared_ptr<RubricItem> createRubricItem();
  std::shared_ptr<RubricItem> createRubricItem(const std::string&);
  std::shared_ptr<RubricItem> createRubricItem(const std::function<bool()>&);
  void run();
  void run_debug();

  bool failed();
  bool passed();

  std::string getFeedback();

  std::vector<std::string> report(); // will switch to json

private:
  std::vector<std::shared_ptr<RubricItem>> items;

  void post_createRubricItem(std::shared_ptr<RubricItem>);
};

#endif