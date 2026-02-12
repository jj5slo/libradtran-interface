class PackedParams{
private:
public:
	ParamAtmosphere* rawAtm;
	ParamAtmosphere* resolvedAtm;
	int              Nheights;
	int              iTOP;
	int              MIN_INDEX;
	int              MAX_INDEX;
	double**         ref_data;
	int              N_ref;
};

double objective_func(const std::vector<double> &Coef, std::vector<double> &grad, void* raw_args);/* for NLopt */
