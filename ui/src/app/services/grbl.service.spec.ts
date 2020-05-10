import { TestBed } from '@angular/core/testing';

import { GrblService } from './grbl.service';

describe('GrblService', () => {
  let service: GrblService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(GrblService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
