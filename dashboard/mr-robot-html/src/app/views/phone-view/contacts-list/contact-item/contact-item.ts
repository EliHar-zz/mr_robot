export class ContactItem {
  constructor(private name : string, private numbers : number[]){}

  public get getName() : string {
    return this.name;
  }

  public get getNumbers() : number[] {
    return this.numbers;
  }
}
