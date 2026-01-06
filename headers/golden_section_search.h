#ifndef __GOLDEN_SECTION_SEARCH_H__
#define __GOLDEN_SECTION_SEARCH_H__




double golden_section_search( double& x, double x_min, double x_max, double xtol, double (*wrapper)(const std::vector<double>&, std::vector<double>&, void*), void* args);



#endif
