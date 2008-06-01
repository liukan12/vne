% generate a shape for import to VNE
% create it as a levelset f(x,y,z) = 0

[xx,yy,zz] = meshgrid(-3:.0625:3,-3:.0625:3,-3:.0625:3);

fxyz = xx.^2 + 4*yy.^2 + zz.^4 - 1.0;

% get faces and vertices
[f,v] = isosurface(xx,yy,zz,fxyz,0.0); 
[nf,nv] = reducepatch(f,v,10000);
n = isonormals(xx,yy,zz,fxyz,nv)

trisurf(nf,nv(:,1),nv(:,2),nv(:,3));

 csvwrite('faces7.dat',nf);
 csvwrite('verts7.dat',nv);
 csvwrite('norms7.dat',n);