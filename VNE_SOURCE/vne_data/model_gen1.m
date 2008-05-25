% generate a shape for import to VNE
% create it as a levelset f(x,y,z) = 0

[xx,yy,zz] = meshgrid(-3:.125:3,-3:.125:3,-3:.125:3);

fxyz = xx.^3+yy.^2+zz.^2 - 2;

% get faces and vertices
[f,v] = isosurface(xx,yy,zz,fxyz,0.0); 
[nf,nv] = reducepatch(f,v,300);
n = isonormals(xx,yy,zz,fxyz,nv)

trisurf(nf,nv(:,1),nv(:,2),nv(:,3))

% csvwrite('faces9.dat',nf);
% csvwrite('verts9.dat',nv);
% csvwrite('norms9.dat',n);