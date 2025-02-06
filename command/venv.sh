cd "$(dirname "${BASH_SOURCE[0]}")"
source ./venv/bin/activate
pip install -r "./requirements.txt"
export PYTHONPATH=$(pwd):$(pwd)/generated  # should be vbex directory
echo "PYTHONPATH=$PYTHONPATH"
