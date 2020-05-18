import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { BabylonComponent } from './babylon.component';

describe('BabylonComponent', () => {
  let component: BabylonComponent;
  let fixture: ComponentFixture<BabylonComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ BabylonComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(BabylonComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
