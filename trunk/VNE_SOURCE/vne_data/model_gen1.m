% generate a shape for import to VNE
% create it as a levelset f(x,y,z) = 0

[xx,yy,zz] = meshgrid(-2:.0625:2,-2:.0625:2,-3:.0625:3);

fxyz = xx.^2 + zz.^2 - 1;

% get faces and vertices
[f,v] = isosurface(xx,yy,zz,fxyz,0.0); 
[nf,nv] = reducepatch(f,v,100);
nv = nv;
n = isonormals(xx,yy,zz,fxyz,nv);
trisurf(nf,nv(:,1),nv(:,2),nv(:,3));

 csvwrite('facesmortar.dat',nf);
 csvwrite('vertsmortar.dat',nv);
 csvwrite('normsmortar.dat',n);