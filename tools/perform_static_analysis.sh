# requires building the docker image first!
# todo: build and retry on failure to find image
# todo: ensure compile_commands.json exists

VERSION=0.0
PROJECT_DIR=$(git rev-parse --show-toplevel)
CODECHECKER_PATH=/root/codechecker/build/CodeChecker/bin/CodeChecker

echo $PROJECT_DIR
REPORT_DIR=$PROJECT_DIR/reports

docker run -it \
    -v $PROJECT_DIR:$PROJECT_DIR \
    codechecker:$VERSION \
    /bin/bash -c ". ~/codechecker/venv/bin/activate && cd $PROJECT_DIR && $CODECHECKER_PATH analyze $PROJECT_DIR/compile_commands.json --enable sensitive -o $REPORT_DIR"