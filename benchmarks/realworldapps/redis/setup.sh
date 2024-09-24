apt-get install -y \
        apache2-utils \
        openjdk-8-jre \
        redis-tools \
        wget


mkdir tools
cd tools && \
    wget https://github.com/brianfrankcooper/YCSB/releases/download/0.17.0/ycsb-redis-binding-0.17.0.tar.gz && \
    tar -zxf ycsb-redis-binding-0.17.0.tar.gz