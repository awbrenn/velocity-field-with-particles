//
// Created by awbrenn on 11/20/16.
//

#include "ObjWriter.h"

bool ObjWriter::writeParticles(vector<Particle> particles, string output_path,
                               string file_base_name, unsigned int frame_number) {

  ofstream objFileStream;
  bool succeeded = true;
  string file_full_path;

  char extension[64];

  sprintf(extension, ".%04u.obj", frame_number);

  file_full_path = output_path + file_base_name + extension;

  cout << "Writing to file:\t" << file_full_path << endl;

  objFileStream.open(file_full_path);
  for (auto p = particles.begin(); p < particles.end(); ++p) {
    objFileStream << "v " << p->pos.x << " " << p->pos.y << " " << p->pos.z << "\n";
  }

  objFileStream.close();
  return succeeded;
}
