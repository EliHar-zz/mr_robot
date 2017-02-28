import { Injectable } from '@angular/core';

@Injectable()
export class DialService {
  private dialNumber : string;
  constructor() {
    this.dialNumber = "";
  }

  public setDialNumber(val : string) {
      this.dialNumber = val;
  }

  public get getDialNumber() {
    return this.dialNumber;
  }
}
