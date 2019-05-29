#section support_code_struct

starry_theano::filter::Filter<DTYPE_OUTPUT_0>* APPLY_SPECIFIC(F);

#section init_code_struct

{
    APPLY_SPECIFIC(F) = NULL;
}

#section cleanup_code_struct

if (APPLY_SPECIFIC(F) != NULL) {
    delete APPLY_SPECIFIC(F);
}

#section support_code_struct

int APPLY_SPECIFIC(filter)(
    PyArrayObject* input0,  // u
    PyArrayObject* input1,  // f
    PyArrayObject** output0,  // F_out (the filter operator, F)
    PARAMS_TYPE* params
) {
    typedef DTYPE_OUTPUT_0 Scalar;

    // Map degree
    int ydeg = params->ydeg;
    int udeg = params->udeg;
    int fdeg = params->fdeg;
    int N = params->N;
    int Ny = params->Ny;
    int Nu = params->Nu;
    int Nf = params->Nf;

    // Get pointers to the input data
    npy_intp nu = params->Nu;
    npy_intp nf = params->Nf;
    auto u = starry_theano::get_flat_input<DTYPE_INPUT_0>(input0, &nu);
    auto f = starry_theano::get_flat_input<DTYPE_INPUT_1>(input1, &nf);
    if (u == NULL || f == NULL) {
        PyErr_Format(PyExc_RuntimeError, "either `u` or `f` is NULL");
        return 1;
    }

    // Set up the op; if it exists, reuse it
    if (APPLY_SPECIFIC(F) == NULL || 
        APPLY_SPECIFIC(F)->ydeg != ydeg ||
        APPLY_SPECIFIC(F)->udeg != udeg ||
        APPLY_SPECIFIC(F)->fdeg != fdeg) {
        if (APPLY_SPECIFIC(F) != NULL)
            delete APPLY_SPECIFIC(F);
        APPLY_SPECIFIC(F) = new starry_theano::filter::Filter<Scalar>(ydeg, udeg, fdeg);
    }

    // Access the output data
    std::vector<npy_intp> shapeNxNy{N, Ny};
    auto F_out = starry_theano::allocate_output<DTYPE_OUTPUT_0>(
        2, &(shapeNxNy[0]), TYPENUM_OUTPUT_0, output0
    );
    if (F_out == NULL) {
        PyErr_Format(PyExc_RuntimeError, "`F` is NULL");
        return 2;
    } 
    
    // Perform the calculation
    // TODO: Optimize this; debug only
    Eigen::Matrix<Scalar, Eigen::Dynamic, 1> u_vec(Nu), f_vec(Nf);
    for (int n = 0; n < Nu; ++n) u_vec(n) = u[n];
    for (int n = 0; n < Nf; ++n) f_vec(n) = f[n];
    APPLY_SPECIFIC(F)->compute(u_vec, f_vec);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < Ny; ++j) {
            F_out[i * Ny + j] = APPLY_SPECIFIC(F)->F(i, j);
        }
    }
    
    return 0;
}