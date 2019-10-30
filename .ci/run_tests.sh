#!/bin/bash

# Load the environment
if [[ -e $CONDA ]]; then
    . $CONDA/etc/profile.d/conda.sh
    conda activate starry
fi

# Install dependencies
pip install -U parameterized nose pytest pytest-cov coverage-badge

# Install starry beta
# TODO: use a wheel!
if ! python -c "import starry_beta" &> /dev/null; then
    git clone https://github.com/rodluger/starry_beta
    pushd starry_beta
    python setup.py develop &> /dev/null
    popd
fi

# Run tests
py.test -v -s tests/greedy --junitxml=junit/test-results-greedy.xml \
        --cov=starry --cov-append --cov-report html:htmlcov tests/greedy
py.test -v -s tests/lazy --junitxml=junit/test-results-lazy.xml --cov=starry \
        --cov-report html:htmlcov tests/lazy
py.test -v -s starry/extensions/tests/greedy \
        --junitxml=junit/test-results-extensions-greedy.xml --cov=starry \
        --cov-append --cov-report html:htmlcov starry/extensions/tests/greedy
py.test -v -s starry/extensions/tests/lazy \
        --junitxml=junit/test-results-extensions-lazy.xml --cov=starry \
        --cov-append --cov-report html:htmlcov starry/extensions/tests/lazy

# Get coverage badge
mkdir -p htmlcov
coverage-badge -o htmlcov/coverage.svg