FROM devkitpro/devkitarm:latest

# Install NFLib
# RUN cd libraries && git clone https://github.com/knightfox75/nds_nflib.git && mv nds_nflib nflib && ln -sT nflib /opt/devkitpro/nflib && cd nflib && make -f Makefile.dkp

RUN git clone https://github.com/GValiente/butano.git && mv butano /workspaces/gbajam24/butano
